package main

import (
	"archive/tar"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"github.com/gorilla/websocket"
	log "github.com/physacco/golib/logger"
	"github.com/vmihailenco/msgpack"
	"io/ioutil"
	"net/url"
	"os"
	"path/filepath"
	"strings"
	"time"
)

const WSEOF = "websocket: close 1006 (abnormal closure): unexpected EOF"

var MessageCounter int = 0
var HaveMadeSaveDir bool = false
var TarStream *tar.Writer = nil

type TSubscribe struct {
	Source string `msgpack:"source"`
	Topic  string `msgpack:"topic"`
	Data   string `msgpack:"data"`
}

type TMessage struct {
	Source string `msgpack:"source"`
	Topic  string `msgpack:"topic"`
	Data   []byte `msgpack:"data"`
	Time   int64  `msgpack:"time"`
}

func Connect() (conn *websocket.Conn, err error) {
	u := url.URL{
		Scheme: "ws",
		Host:   ServerAddr,
		Path:   "/",
	}

	log.Info("connect to %s", u.String())

	conn, _, err = websocket.DefaultDialer.Dial(u.String(), nil)
	return
}

func DoSubscription(conn *websocket.Conn) {
	topics := strings.Join(SubTopics, ",")
	log.Info("subscribe to %s", topics)

	for _, topic := range SubTopics {
		SubscribeOneTopic(conn, topic)
	}
}

func SubscribeOneTopic(conn *websocket.Conn, topic string) {
	msg := &TSubscribe{
		Source: ClientName,
		Topic:  "subscribe",
		Data:   topic,
	}

	data, err := msgpack.Marshal(msg)
	if err != nil {
		log.Fatal("msgpack.Marshal: %v", err)
	}

	err = conn.WriteMessage(websocket.BinaryMessage, data)
	if err != nil {
		log.Fatal("WriteMessage: %v", err)
	}
}

func setReadTimeout(conn *websocket.Conn, nsecs int) bool {
	if err := conn.SetReadDeadline(AfterSeconds(nsecs)); err != nil {
		log.Error("SetReadDeadline: %v", err)
		return false
	} else {
		return true
	}
}

func CloseConn(conn *websocket.Conn) {
	if err := conn.Close(); err != nil {
		log.Error("conn.Close: %v", err)
	}
}

func DoReading(conn *websocket.Conn) {
	defer func() {
		CloseConn(conn)
	}()

	for {
		if !setReadTimeout(conn, 3600) {
			return
		}

		msgtype, data, err := conn.ReadMessage()
		if err != nil {
			if err.Error() != WSEOF {
				log.Error("ReadMessage: %v", err)
			}

			return
		}

		MessageCounter += 1
		HandleMessage(msgtype, data)

		if Limit > 0 && MessageCounter >= Limit {
			break
		}
	}
}

func HandleMessage(msgtype int, data []byte) {
	switch msgtype {
	case websocket.TextMessage:
		log.Error("unexpected message: %s", string(data))
	case websocket.BinaryMessage:
		HandleBinaryMessage(data)
	}
}

func HandleBinaryMessage(data []byte) {
	var msg TMessage
	err := msgpack.Unmarshal(data, &msg)
	if err != nil {
		log.Error("msgpack.Unmarshal: %v", err)
		return
	}

	ts := FormatMessageTime(&msg)
	fmt.Printf("[%d] %s @%s: \"%s\" (%d)\n",
		MessageCounter, ts, msg.Source, msg.Topic, len(msg.Data))

	if IsStringWildcardMatchArray(msg.Topic, TextTopics) {
		ShowTextMessage(&msg)
	} else if IsStringWildcardMatchArray(msg.Topic, MsgpackTopics) {
		ShowMsgpackMessage(&msg)
	} else {
		ShowBinaryMessage(&msg)
	}

	if IsStringWildcardMatchArray(msg.Topic, SaveTopics) {
		SaveMessage(&msg)
	}
}

func ShowTextMessage(msg *TMessage) {
	fmt.Printf("%s\n", msg.Data)
}

func ShowMsgpackMessage(msg *TMessage) {
	var v interface{}

	err := msgpack.Unmarshal(msg.Data, &v)
	if err != nil {
		log.Error("msgpack.Unmarshal: %v", err)
		return
	}

	fmt.Printf("%v\n", v)
}

func ShowBinaryMessage(msg *TMessage) {
	total_size := len(msg.Data)
	slice_size := Min(total_size, 1024)
	slice := msg.Data[:slice_size]
	fmt.Printf("%s", hex.Dump(slice))

	if msg.Topic == "output.can.0x700" {
		can_info := ParseCanInfo(msg.Data, msg.Time)
		json_data, _ := json.Marshal(can_info)
		fmt.Printf("AS JSON: %s\n", string(json_data))
	} else if msg.Topic == "output.can.0x760" {
		car_info := ParseCarInfo(msg.Data, msg.Time)
		json_data, _ := json.Marshal(car_info)
		fmt.Printf("AS JSON: %s\n", string(json_data))
	}
}

func FormatMessageTime(msg *TMessage) string {
	tm := MakeTimeFromMicroSeconds(msg.Time)
	return tm.Format("2006-01-02T15:04:05.000000Z07:00")
}

func SaveMessage(msg *TMessage) {
	filename := fmt.Sprintf("flowout.%d-%s-%s.dat",
		msg.Time, msg.Source, msg.Topic)

	if SaveDir != "" {
		SaveMessageToDir(msg, filename)
	}

	if SaveTar != "" {
		SaveMessageToTar(msg, filename)
	}
}

func MakeSaveDir() {
	if SaveDir == "" || SaveDir == "." || SaveDir == ".." {
		return
	}

	err := os.Mkdir(SaveDir, 0755)
	if err != nil && !os.IsExist(err) {
		log.Fatal("%v", err)
	}
}

func SaveMessageToDir(msg *TMessage, filename string) {
	if !HaveMadeSaveDir {
		MakeSaveDir()
		HaveMadeSaveDir = true
	}

	filepath := filepath.Join(SaveDir, filename)
	err := ioutil.WriteFile(filepath, msg.Data, 0644)
	if err != nil {
		log.Error("%v", err)
	}
}

func SaveMessageToTar(msg *TMessage, filename string) {
	if TarStream == nil {
		flag := os.O_WRONLY | os.O_CREATE | os.O_TRUNC // 'w'
		f, err := os.OpenFile(SaveTar, flag, 0644)
		if err != nil {
			log.Fatal("%v", err)
		}

		TarStream = tar.NewWriter(f)
	}

	now := time.Now()
	hdr := &tar.Header{
		Name:       filename,
		Size:       int64(len(msg.Data)),
		Mode:       0644,
		ModTime:    now,
		AccessTime: now,
		ChangeTime: now,
	}

	if err := TarStream.WriteHeader(hdr); err != nil {
		log.Error("%v", err)
	}

	if _, err := TarStream.Write([]byte(msg.Data)); err != nil {
		log.Error("%v", err)
	}

	if err := TarStream.Flush(); err != nil {
		log.Error("%v", err)
	}
}
