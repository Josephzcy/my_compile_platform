#!/usr/bin/python3
import datetime
import re

def parser_apa_can_log(can_log_file, flag="e_can"):

    wf = open(can_log_file + '_result.txt', "w")
    def e_can(line):
        fields = line.strip().split()
        can_id, data = int(fields[2], 16), fields[4:]
        if 'remote' in data:
            return None, None, None
        print(can_id, data)
        return float(fields[0][1:-1]), can_id, data

    def can_oe(line):
        reg = r'(\w+)\W+Rx\W+d\W+[8|5]\W+(.*?)\W+Length'
        xd = re.findall(reg, line)
        if len(xd) == 0:
            return None, None, None
        can_id = int(xd[0][0], 16)
        data = xd[0][1].strip().split()
        print(can_id, data)
        return float(line.split()[0]), can_id, data

    def zhouligong(line):
        pass

    def cve(line):
        if "CAN" not in line:
            return None, None, None
        fields = line.strip().split()
        can_id = int(fields[3], 16)
        data = fields[-8:]
        ts = int(fields[0]) + int(fields[1]) / 1e6
        # print(can_id, data)
        return ts, can_id, data

    c = 0
    with open(can_log_file, "r") as rf:
        for line in rf:
            data = None
            if flag == 'e_can':
                ts, can_id, data = e_can(line)
            elif flag == 'can_oe':
                if "date" in line:
                    c = datetime.datetime.strptime(line.strip(), "date %a %b %d %H:%M:%S.%f %p %Y").timestamp()
                    continue
                ts, can_id, data = can_oe(line)
                if data:
                    ts = ts + c
            elif flag == 'cve':
                ts, can_id, data = cve(line)

            if data is None:
                continue

            if can_id == 0x121:

                if len(data) < 2:
                    continue

                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf

                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                # print(len(v), '------')
                # reversed(v)
                v = int(v, 2)
                if v == 0xFFF or int(pre[0]):
                    continue
                v = v * 0.06875
                print(ts, 'vehicle speed', v, 'km/h', file = wf)
            elif can_id == 0x1F0:

                if len(data) < 8:
                    continue

                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                fl = int(v, 2) * 0.06875

                suf = bin(int(data[2], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[3], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                fr = int(v, 2) * 0.06875

                suf = bin(int(data[3], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[4], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre + suf[:4]
                rl = int(v, 2) * 0.06875

                suf = bin(int(data[5], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[6], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                rr = int(v, 2) * 0.06875

                print(ts, 'WheelSpeed_FL', fl, 'km/h', file=wf)
                print(ts, 'WheelSpeed_FR', fr, 'km/h', file=wf)
                print(ts, 'WheelSpeed_RL', rl, 'km/h', file=wf)
                print(ts, 'WheelSpeed_RR', rr, 'km/h', file=wf)
                pass

            elif can_id == 0x212:
                if len(data) < 8:
                    continue
                suf = bin(int(data[6], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                asm = int(suf[-4:], 2)
                if asm == 0:
                    continue
                print(ts, 'The actual shift mode', asm, file=wf)
            elif can_id == 0x11F:
                if len(data) < 2:
                    continue
                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = int(pre + suf, 2)

                if int(pre[0]) == 1:
                    v = 0xFFFF - v + 1
                    v = -v

                if v == 0x7FFF:
                    continue
                v = v * 0.1
                if v < -780 or v > 779.9:
                    continue
                print(ts, 'angle', v, file=wf)

            elif can_id == 0x342:
                if len(data) < 3:
                    continue

                suf = bin(int(data[3], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                asm = int(suf[-4:], 2)
                if asm == 0:
                    continue
                print(ts, 'The actual shift mode', asm, file=wf)

            elif can_id == 0x122:

                if len(data) < 8:
                    continue

                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                fl = int(v, 2) * 0.06875

                suf = bin(int(data[2], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[3], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                fr = int(v, 2) * 0.06875

                suf = bin(int(data[4], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[5], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                rl = int(v, 2) * 0.06875

                suf = bin(int(data[6], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[7], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-4:] + suf
                rr = int(v, 2) * 0.06875

                print(ts, 'WheelSpeed_FL', fl, 'km/h', file=wf)
                print(ts, 'WheelSpeed_FR', fr, 'km/h', file=wf)
                print(ts, 'WheelSpeed_RL', rl, 'km/h', file=wf)
                print(ts, 'WheelSpeed_RR', rr, 'km/h', file=wf)
                pass
            elif can_id == 0x422:
                if len(data) < 8:
                    continue
                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-2:] + suf
                flc = int(v, 2)


                suf = bin(int(data[2], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[3], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-2:] + suf
                frc = int(v, 2)

                suf = bin(int(data[3], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[4], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-6:] + suf[:4]
                rlc = int(v, 2)

                suf = bin(int(data[5], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[6], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = pre[-2:] + suf
                rrc = int(v, 2)

                print(ts, 'FL Wheel Pulse Counter', flc, file=wf)
                print(ts, 'FR Wheel Pulse Counter', frc, file=wf)
                print(ts, 'RL Wheel Pulse Counter', rlc, file=wf)
                print(ts, 'RR Wheel Pulse Counter', rrc, file=wf)

            elif can_id == 0x614:
                # print('--------------')
                if len(data) < 8:
                    continue
                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                slf = int(suf, 2)
                print(ts, "short dis radar fl", slf, file=wf)

                suf = bin(int(data[1], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                slmf = int(suf, 2)

                print(ts, "short dis radar flm", slmf, file=wf)

                suf = bin(int(data[2], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf

                srmf = int(suf, 2)
                print(ts, "short dis radar frm", srmf, file=wf)

                suf = bin(int(data[3], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                srf = int(suf, 2)
                print(ts, "short dis radar fr", srf, file=wf)

                suf = bin(int(data[4], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                slb = int(suf, 2)
                print(ts, "short dis radar bl", slb, file=wf)

                suf = bin(int(data[5], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf

                slmb = int(suf, 2)
                print(ts, "short dis radar blm", slmb, file=wf)

                suf = bin(int(data[6], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf

                srmb = int(suf, 2)
                print(ts, "short dis radar brm", srmb, file=wf)

                suf = bin(int(data[7], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                srb = int(suf, 2)
                print(ts, "short dis radar br", srb, file=wf)

            elif can_id == 0x615:
                if len(data) < 8:
                    continue
                suf = bin(int(data[2], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[3], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = int(suf + pre, 2)
                print(ts, "long dis radar fl", v, file=wf)

                suf = bin(int(data[0], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[1], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = int(suf + pre, 2)
                print(ts, "long dis radar fr", v, file=wf)

                suf = bin(int(data[4], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[5], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = int(suf + pre, 2)
                print(ts, "long dis radar bl", v, file=wf)

                suf = bin(int(data[6], 16))[2:]
                suf = suf if len(suf) >= 8 else '0' * (8 - len(suf)) + suf
                pre = bin(int(data[7], 16))[2:]
                pre = pre if len(pre) >= 8 else '0' * (8 - len(pre)) + pre
                v = int(suf + pre, 2)
                print(ts, "long dis radar br", v, file=wf)


if __name__ == '__main__':
    import sys
    if len(sys.argv) >= 3:
        parser_apa_can_log(sys.argv[1], sys.argv[2])
