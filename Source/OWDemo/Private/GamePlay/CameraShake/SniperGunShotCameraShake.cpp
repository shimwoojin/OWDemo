// If you want to use this project, mail to ggoggal@gmail.com

#include "SniperGunShotCameraShake.h"

USniperGunShotCameraShake::USniperGunShotCameraShake()
{
	OscillationDuration = 1.0f;
	OscillationBlendInTime = .05f;
	OscillationBlendOutTime = .05f;

	RotOscillation.Pitch.Amplitude = 5.f;
	RotOscillation.Pitch.Frequency = PI;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}