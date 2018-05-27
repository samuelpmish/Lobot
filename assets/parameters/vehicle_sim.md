
### .uc class
```
/** Data for each wheel. */
var() const export editinline array Wheels;
/** Max turn angle (degrees) based on speed */
var() @NULL SteerAngleCurve;
/** Max drive torque from engine */
var() float DriveTorque;
/** Scale how much drive torque goes to the wheels based on speed */
var() @NULL DriveTorqueCurve;
/** Max brake torque from wheels */
var() float BrakeTorque;
/** Scale how much brake torque goes to the wheels based on speed */
var() @NULL BrakeTorqueCurve;
/** Max handbrake turn angle (degrees) based on speed */
var() @NULL HandbrakeSteerAngleCurve;
/** How fast our output Handbrake can rise and fall */
var() @NULL HandbrakeRate;
/** Speed at which to automatically put on the brakes */
var() float StopThreshold;
/** Multiplier for brake output when there is no throttle input */
var() float IdleBrakeFactor;
/** Multiplier for brake output when traveling forward but throttling backwards, and vice-versa */
var() float OppositeBrakeFactor;
/** Do ackermann steering? */
var() bool bUseAckermannSteering;
```


### SDK Class
```
// Class TAGame.VehicleSim_TA
// 0x007F (0x00E4 - 0x0065)
class UVehicleSim_TA : public UActorComponent
{
public:
	unsigned char                                      UnknownData00[0x3];                                       // 0x0065(0x0003) MISSED OFFSET
	TArray<class UWheel_TA*>                           Wheels;                                                   // 0x0068(0x000C) (Edit, Const, ExportObject, Component, NeedCtorLink, EditInline)
	struct FInterpCurveFloat                           SteerAngleCurve;                                          // 0x0074(0x0010) (Edit, NeedCtorLink)
	float                                              DriveTorque;                                              // 0x0084(0x0004) (Edit)
	struct FInterpCurveFloat                           DriveTorqueCurve;                                         // 0x0088(0x0010) (Edit, NeedCtorLink)
	float                                              BrakeTorque;                                              // 0x0098(0x0004) (Edit)
	struct FInterpCurveFloat                           HandbrakeSteerAngleCurve;                                 // 0x009C(0x0010) (Edit, NeedCtorLink)
	struct FInputRate                                  HandbrakeRate;                                            // 0x00AC(0x0008) (Edit)
	float                                              StopThreshold;                                            // 0x00B4(0x0004) (Edit)
	float                                              IdleBrakeFactor;                                          // 0x00B8(0x0004) (Edit)
	float                                              OppositeBrakeFactor;                                      // 0x00BC(0x0004) (Edit)
	unsigned long                                      bUseAckermannSteering : 1;                                // 0x00C0(0x0004) (Edit)
	unsigned long                                      bWasAttached : 1;                                         // 0x00C0(0x0004) (Const, Transient)
	float                                              OutputThrottle;                                           // 0x00C4(0x0004) (Transient)
	float                                              OutputSteer;                                              // 0x00C8(0x0004) (Transient)
	float                                              OutputBrake;                                              // 0x00CC(0x0004) (Transient)
	float                                              OutputHandbrake;                                          // 0x00D0(0x0004) (Transient)
	class AVehicle_TA*                                 Vehicle;                                                  // 0x00D4(0x0004) (Transient)
	class ACar_TA*                                     Car;                                                      // 0x00D8(0x0004) (Transient)
	struct FPointer                                    BulletVehicle;                                            // 0x00DC(0x0004) (Const, Native, Transient)
	float                                              SteeringSensitivity;                                      // 0x00E0(0x0004) (Transient)
```

```
// Enum Core.Object.EInterpCurveMode
enum class EInterpCurveMode : uint8_t
{
	CIM_Linear                     = 0,
	CIM_CurveAuto                  = 1,
	CIM_Constant                   = 2,
	CIM_CurveUser                  = 3,
	CIM_CurveBreak                 = 4,
	CIM_CurveAutoClamped           = 5,
	CIM_MAX                        = 6
};


// Enum Core.Object.EInterpMethodType
enum class EInterpMethodType : uint8_t
{
	IMT_UseFixedTangentEvalAndNewAutoTangents = 0,
	IMT_UseFixedTangentEval        = 1,
	IMT_UseBrokenTangentEval       = 2,
	IMT_MAX                        = 3
};

struct FInterpCurvePointFloat
{
	float                                              InVal;                                                    // 0x0000(0x0004) (Edit)
	float                                              OutVal;                                                   // 0x0004(0x0004) (Edit)
	float                                              ArriveTangent;                                            // 0x0008(0x0004) (Edit)
	float                                              LeaveTangent;                                             // 0x000C(0x0004) (Edit)
	TEnumAsByte<EInterpCurveMode>                      InterpMode;                                               // 0x0010(0x0001) (Edit)
};

struct FInterpCurveFloat
{
	TArray<struct FInterpCurvePointFloat>              Points;                                                   // 0x0000(0x000C) (Edit, NeedCtorLink)
	TEnumAsByte<EInterpMethodType>                     InterpMethod;                                             // 0x000C(0x0001)
};
```

#### FInterpCurveFloat SteerAngleCurve
```
TArray<struct FInterpCurvePointFloat> Data pointer    *
TArray<struct FInterpCurvePointFloat> length          6
TArray<struct FInterpCurvePointFloat> size            6
EInterpMethodType                                     0
------
Data:
0, 29.92381096, 0, 0, 0
500, 18.0187664, 0, 0, 0
1000, 10.32923031, 0, 0, 0
1500, 6.019903183, 0, 0, 0
1750, 4.850355625, 0, 0, 0
3000, 1.97511673, 0, 0, 0,
```

#### float DriveTorque
```
288000
```

#### FInterpCurveFloat DriveTorqueCurve
```
TArray<struct FInterpCurvePointFloat> Data pointer    *
TArray<struct FInterpCurvePointFloat> length          5
TArray<struct FInterpCurvePointFloat> size            21
EInterpMethodType                                     0
------
Data:
-1410, 0, 0, 0, 0
-1400, 0.1000000015, 0, 0, 0
0, 1, 0, 0, 0
1400, 0.1000000015, 0, 0, 0
1410, 0, 0, 0, 0
```

#### float BrakeTorque
```
10500
```

#### FInterpCurveFloat HandbrakeSteerAngleCurve
```
TArray<struct FInterpCurvePointFloat> Data pointer    *
TArray<struct FInterpCurvePointFloat> length          2
TArray<struct FInterpCurvePointFloat> size            2
EInterpMethodType                                     0
------
Data:
0, 22.08690262, 0, 0, 0
2500, 7.173928738, 0, 0, 0
```

#### FInputRate HandbrakeRate
```
float RiseRate  5
float FallRate  2
```

#### float StopThreshold
```
25
```

#### float IdleBrakeFactor
```
0.1500000006
```

#### float OppositeBrakeFactor
```
1
```

#### bool bUseAckermannSteering
```
true ?
```

#### float SteeringSensitivity
```
1
```
