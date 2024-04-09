package main

type CanInfo struct {
	SoundType           byte  `json:"sound_type"`
	TimeIndicator       byte  `json:"time_indicator"`
	ZeroSpeed           byte  `json:"zero_speed"`
	HeadwayValid        byte  `json:"headway_valid"`
	HeadwayMeasurement  byte  `json:"headway_measurement"`
	NoError             byte  `json:"no_error"`
	ErrorCode           byte  `json:"error_code"`
	LDWOff              byte  `json:"ldw_off"`
	LeftLDW             byte  `json:"left_ldw"`
	RightLDW            byte  `json:"right_ldw"`
	FCWOn               byte  `json:"fcw_on"`
	Maintenance         byte  `json:"maintenance"`
	Failsafe            byte  `json:"failsafe"`
	PedsFCW             byte  `json:"peds_fcw"`
	PedsInDz            byte  `json:"peds_in_dz"`
	TamperAlert         byte  `json:"tamper_alert"`
	TSREnabled          byte  `json:"tsr_enabled"`
	TSRWarningLevel     byte  `json:"tsr_warning_level"`
	HeadwayWarningLevel byte  `json:"headway_warning_level"`
	HWRepeatableEnabled byte  `json:"hw_repeatable_enabled"`
	Time                int64 `json:"time"`
}

type CarInfo struct {
	BrakeSignal       byte  `json:"brake_signal"`
	LeftTurn          byte  `json:"left_turn"`
	RightTurn         byte  `json:"right_turn"`
	Wipers            byte  `json:"wipers"`
	LowBeam           byte  `json:"low_beam"`
	HighBeam          byte  `json:"high_beam"`
	WipersAvailable   byte  `json:"wipers_available"`
	LowBeamAvailable  byte  `json:"low_beam_available"`
	HighBeamAvailable byte  `json:"high_beam_available"`
	SpeedAvailable    byte  `json:"speed_available"`
	Speed             byte  `json:"speed"`
	Time              int64 `json:"time"`
}

func ParseCanInfo(buf []byte, time int64) CanInfo {
	return CanInfo{
		// byte 0
		SoundType:     buf[0] & 0x7,
		TimeIndicator: (buf[0] >> 3) & 0x3,
		// byte 1
		ZeroSpeed: (buf[1] >> 5) & 0x1,
		// byte 2
		HeadwayValid:       buf[2] & 0x1,
		HeadwayMeasurement: buf[2] >> 1,
		// byte 3
		NoError:   buf[3] & 0x1,
		ErrorCode: buf[3] >> 1,
		// byte 4
		LDWOff:      buf[4] & 0x1,
		LeftLDW:     (buf[4] >> 1) & 0x1,
		RightLDW:    (buf[4] >> 2) & 0x1,
		FCWOn:       (buf[4] >> 3) & 0x1,
		Maintenance: (buf[4] >> 6) & 0x1,
		Failsafe:    (buf[4] >> 7) & 0x1,
		// byte 5
		PedsFCW:     (buf[5] >> 1) & 0x1,
		PedsInDz:    (buf[5] >> 2) & 0x1,
		TamperAlert: (buf[5] >> 5) & 0x1,
		TSREnabled:  (buf[5] >> 7) & 0x1,
		// byte 6
		TSRWarningLevel: buf[6] & 0x7,
		// byte 7
		HeadwayWarningLevel: buf[7] & 0x3,
		HWRepeatableEnabled: (buf[7] >> 2) & 0x1,
		Time:                time,
	}
}

func ParseCarInfo(buf []byte, time int64) CarInfo {
	return CarInfo{
		// byte 0
		BrakeSignal: (buf[0] >> 0) & 1,
		LeftTurn:    (buf[0] >> 1) & 1,
		RightTurn:   (buf[0] >> 2) & 1,
		Wipers:      (buf[0] >> 3) & 1,
		LowBeam:     (buf[0] >> 4) & 1,
		HighBeam:    (buf[0] >> 5) & 1,
		// byte 1
		WipersAvailable:   (buf[1] >> 3) & 1,
		LowBeamAvailable:  (buf[1] >> 4) & 1,
		HighBeamAvailable: (buf[1] >> 5) & 1,
		SpeedAvailable:    (buf[1] >> 7) & 1,
		// byte 2
		Speed: buf[2],
		Time:  time,
	}
}
