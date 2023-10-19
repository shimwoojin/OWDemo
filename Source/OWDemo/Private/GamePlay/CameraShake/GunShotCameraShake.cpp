// If you want to use this project, mail to ggoggal@gmail.com

#include "GunShotCameraShake.h"

UGunShotCameraShake::UGunShotCameraShake()
{
	OscillationDuration = .25f;
	OscillationBlendInTime = .05f;
	OscillationBlendOutTime = .05f;

	//RotOscillation.Pitch.Amplitude = 20.f;
	//RotOscillation.Pitch.Frequency = .25f;
	//RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
	RotOscillation.Yaw.Amplitude = 1.2f;
	RotOscillation.Yaw.Frequency = PI * .25f;

	//LocOscillation.Y.Amplitude = 5.f;
	//LocOscillation.Y.Frequency= .25f;
	LocOscillation.Z.Amplitude = 20.f;
	LocOscillation.Z.Frequency = PI * .25f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}