T1Q_ADDRESS=root@192.168.66.234:/app/algo/apa

scp script/run_bev.sh ${T1Q_ADDRESS}/script

scp flag/bev.flag ${T1Q_ADDRESS}/flag

scp build/tda4/bin/bird_eye_view ${T1Q_ADDRESS}/bin
