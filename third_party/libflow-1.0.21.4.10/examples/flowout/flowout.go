package main

import (
	"flag"
	"fmt"
	log "github.com/physacco/golib/logger"
	"runtime"
)

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

const Program = "flowout"
const Version = "1.0.6"
const Usage = `Usage: %s [Options]

Options:
  --help                  print this help message and exit
  --version               print the version and exit
  --server ADDRESS        specify the server (def: 127.0.0.1:24012)
  --name NAME             specify the client's name (def: %s)
  --sub TOPICs            specify the topics to subscribe
  --text TOPICs           specify the topics to print as text
  --msgpack TOPICs        specify the topics to print as msgpack
  --save TOPICs           specify the topics to save
                            (separate topics with comma)
  --save-dir DIR          specify where to save the messages (def: .)
  --limit NUM             specify how many messages to recv (def: inf)

`

//////////////////////////////////////////////////////////////////////
// Arguments
//////////////////////////////////////////////////////////////////////

var ServerAddr string
var ClientName string
var SubTopics []string
var TextTopics []string
var MsgpackTopics []string
var SaveTopics []string
var SaveDir string
var SaveTar string
var Limit int

//////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////

func main() {
	help := flag.Bool("help", false, "print this help message and exit")
	version := flag.Bool("version", false, "print the version and exit")
	server := flag.String("server", "127.0.0.1:24012", "specify the server")
	name := flag.String("name", Program, "specify the client's name")
	sub := flag.String("sub", "*", "specify the topics to subscribe")
	text := flag.String("text", "", "specify the topics to print as text")
	msgpack := flag.String("msgpack", "", "specify the topics to print as msgpack")
	save := flag.String("save", "", "specify the topics to save")
	save_dir := flag.String("save-dir", "", "specify where to save the messages")
	save_tar := flag.String("save-tar", "", "save the messages into a tar file")
	limit := flag.Int("limit", 0, "specify how many messages to recv")

	flag.Parse()

	if *help {
		fmt.Printf(Usage, Program, Program)
		return
	}

	if *version {
		fmt.Printf("%s %s (%s) [%s-%s] (%s)\n", Program, Version, BuildTime,
			runtime.GOOS, runtime.GOARCH, runtime.Version())
		return
	}

	ServerAddr = *server
	ClientName = *name
	SubTopics = ParseCommaSeparatedStrings(*sub)
	TextTopics = ParseCommaSeparatedStrings(*text)
	MsgpackTopics = ParseCommaSeparatedStrings(*msgpack)
	SaveTopics = ParseCommaSeparatedStrings(*save)
	SaveDir = *save_dir
	SaveTar = *save_tar
	Limit = *limit

	conn, err := Connect()
	if err != nil {
		log.Fatal("%v", err)
	}

	DoSubscription(conn)
	DoReading(conn)
}
