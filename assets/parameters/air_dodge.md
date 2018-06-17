## AirDodge

### Values and classes

#### Car
```
MaxLinearSpeed = 2300
```
#### DodgeComponent
```
DodgeInputThreshold                 0.5
SideDodgeImpulse                    90000
SideDodgeImpulseMaxSpeedScale       1.899999976       1.9
ForwardDodgeImpulse                 90000
ForwardDogeImpulseMaxSpeedScale     1
BackwardDodgeImpulse                96000
BackwardDodgeImpulseMaxSpeedScale   2.5
SideDodgeTorque                     260
ForwardDodgeTorque                  224
DodgeTorqueTime                     0.6499999762      0.65
MinDodgeTorqueTime                  0.04100000113     0.041
DodgeZDamping                       0.349999994       0.35
DodgeZDampingDelay                  0.1500000006      0.15
DodgeZDampingUpTime                 0.05999999866     0.06
DodgeImpulseScale                   1
DodgeTorqueScale                    1

enum class ForceMode : uint8_t
{
	ForceMode_Force                = 0,
	ForceMode_Impulse              = 1,
	ForceMode_Velocity             = 2,
	ForceMode_SmoothImpulse        = 3,
	ForceMode_SmoothVelocity       = 4,
	ForceMode_Acceleration         = 5,
	ForceMode_MAX                  = 6
};
```

### Init
Q: Positive X direction if we pitch backwards?
```
DodgeDirection.X = -Car.Input.Pitch;
DodgeDirection.Y = Car.Input.Yaw + Car.Input.Roll;
DodgeDirection = Normal(DodgeDirection);
DodgeTorque.Y = (ForwardDodgeTorque * DodgeDirection.X) * DodgeTorqueScale;
DodgeTorque.X = (SideDodgeTorque * -DodgeDirection.Y) * DodgeTorqueScale;
```

### `CanActivate()  // Decides directional dodge`
If sum of the rotational inputs is >= 0.5, do the directional dodge, otherwise does nothing (and double jump happens, or it happens always?)
```
(Abs(Car.Input.Pitch) + Abs(Car.Input.Yaw)) + Abs(Car.Input.Roll) >= DodgeInputThreshold;
```

### Once activated
- Applies Dodge Impulse once
- Applies ApplyTorqueForces until either on ground or if active time is <= DodgeTorqueTime (0.65s)

### `ApplyDodgeImpulse`
Uses DodgeDirection from setup
Uses ForceMode_Impulse (1)
```
Car.AddForce(GetDodgeImpulse(DodgeDirection), 1);
```

### `GetDodgeImpulse(DodgeDirection)`
Save forward speed of car
```
ForwardSpeed = Car.GetForwardSpeed();
```

Nullify dodge direction unless it is larger than 0.10
```
DodgeDir.X = ((Abs(DodgeDir.X) > 0.10) ? DodgeDir.X : 0.0);
DodgeDir.Y = ((Abs(DodgeDir.Y) > 0.10) ? DodgeDir.Y : 0.0);
```

Deciding if we should do backwards dodge
1. If the forward speed is small enough (<100.0), do backwards dodge if the X direction is smaller than zero
```
if(Abs(ForwardSpeed) < 100.0) {
  bBackwardsDodge = DodgeDir.X < 0.0;
}
```
2. If the forward speed is large enough (>=100.0), we do backwards dodge if we aim at the other direction from the speed
```
else {
  bBackwardsDodge = (DodgeDir.X >= 0.0) != (ForwardSpeed >= 0.0);
}
```
Setting speed scale
```
SpeedScale = Abs(ForwardSpeed) / Car.MaxLinearSpeed;  // Can be larger than 1.0 ?
```

Applying the impulse
Basically, take the base impulse strength using input rotations, and multiply by a speed factor (always 1.0 for forward, 1.0 - 2.5 for backward, 1.0 - 1.9 for sideways)
1. X direction
- if backwards, apply using BackwardDodgeImpulse
```
Impulse.X = ((DodgeDir.X * BackwardDodgeImpulse) * DodgeImpulseScale) * Lerp(1.0, BackwardDodgeImpulseMaxSpeedScale, SpeedScale);
```
- if forward, apply using ForwardDodgeImpulse
```
Impulse.X = ((DodgeDir.X * ForwardDodgeImpulse) * DodgeImpulseScale) * Lerp(1.0, ForwardDodgeImpulseMaxSpeedScale, SpeedScale);
```
2. Y direction
```
Impulse.Y = ((DodgeDir.Y * SideDodgeImpulse) * DodgeImpulseScale) * Lerp(1.0, SideDodgeImpulseMaxSpeedScale, SpeedScale);
```
By setting rotation values between 0 and 1, we can control the force the dodge gives us for our current speed.

Rotate the impulse to be aligned with the car rotation
```
Impulse = Impulse >> MakeRotator(0, Car.RBState.Rotation.Yaw, 0);
```

### `ApplyTorqueForces`
These are applied until on the ground or ActiveTime is > DodgeTorqueTime
These are the inits
```
DodgeTorque.Y = (ForwardDodgeTorque * DodgeDirection.X) * DodgeTorqueScale;
DodgeTorque.X = (SideDodgeTorque * -DodgeDirection.Y) * DodgeTorqueScale;
```
The Y torque is clamped between 0.0 and 1.0 or -1.0 and 0.0 respectively, depending on the X dodge direction
```
ScaledDodgeTorque = DodgeTorque;
if(ScaledDodgeTorque.Y > 0.0) {
  ScaledDodgeTorque.Y *= (1.0 - FClamp(Car.Input.Pitch, 0.0, 1.0));
} else {
  ScaledDodgeTorque.Y *= (1.0 + FClamp(Car.Input.Pitch, -1.0, 0.0));
}
```
The actual torque addition is added with ForceMode.Acceleration (5), again rotated in the direction of the car
! (Not sure what Car.Mesh.Scale is)
```
Car.AddTorque(((float(1) / Car.Mesh.Scale) * ScaledDodgeTorque) >> Car.RBState.Rotation, 5);
```

And if the active time is larger than the DodgeZDamplingDelay (0.15),
and if we're in the time window for damping (DodgeZDampingUpTime == 0.06) OR we're falling, we get bonus -Z force of the Velocity (2) type
```
if(ActiveTime >= DodgeZDampingDelay){
    if((Car.RBState.LinearVelocity.Z < 0.0) || ActiveTime < (DodgeZDampingDelay + DodgeZDampingUpTime)) {
        Car.AddForce(MakeVector(0.0, 0.0, -Car.RBState.LinearVelocity.Z * DodgeZDamping), 2);
    }
}
```
