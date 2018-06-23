(* ::Package:: *)

BeginPackage["RLBotIO`", {"RLBotMisc`"}]

defaultCar::usage = "base car state";
defaultInput::usage = "base controller input";
ImportEpisode::usage = "Import formatted CSV file of car states and inputs";
ImportPredictions::usage = "Import formatted CSV file of car dynamics";

Begin["`Private`"];

defaultCar = <|
"time"->0,
"pos"-> {0, 0, 0},
"vel"-> {0, 0, 0},
"euler"-> {0, 0, 0},
"\[Theta]"-> IdentityMatrix[3],
"\[Omega]"-> {0, 0, 0},
"sup"-> 0,
"jump"-> 0,
"dblj"-> 0,
"grnd"-> 1,
"boost"-> 0,
"\[Kappa]" -> 0.0,
"wheelAngle" -> 0.0,
"jumpTimer" -> 0.0,
"torque" -> {0.0, 0.0, 0.0},
"accel" -> {0.0, 0.0, 0.0}
|>;

defaultInput = <|
"time" ->0.0,
"thr"-> 0.0,
"steer"-> 0.0,
"pitch"-> 0.0,
"yaw"-> 0.0,
"roll"-> 0.0,
"jump"-> 0,
"boost"-> 0,
"brake"-> 0
|>;

ImportEpisode[filename_] := Module[{data, n , car, inputs},
data = Import[filename, "CSV"];
n = Length[data];
car = {};
inputs = {};
Do[
AppendTo[car, <|
"time"->data[[i, 1]],
"pos"-> data[[i,2;;4]],
"vel"-> data[[i,5;;7]],
"euler" -> data[[i, 8;;10]],
"\[Theta]"-> EulerAnglesToMatrix[data[[i, 8;;10]] * \[Pi]/32768.0],
"\[Omega]"-> data[[i,11;;13]],
"sup"-> data[[i,14]],
"jump"-> data[[i,15]],
"dblj"-> data[[i,16]],
"grnd"-> data[[i,17]],
"boost"-> data[[i,18]],
"\[Kappa]" -> 0.0,
"wheelAngle" -> 0.0,
"jumpTimer" -> 0.0,
"torque" -> {0.0, 0.0, 0.0},
"accel" -> {0.0, 0.0, 0.0}
|>];
AppendTo[inputs, <|
"time" -> data[[i, 1]],
"thr"-> data[[i,19]],
"steer"-> data[[i,20]],
"pitch"-> data[[i,21]],
"yaw"-> data[[i,22]],
"roll"-> data[[i,23]],
"jump"-> data[[i,24]],
"boost"-> data[[i,25]],
"brake"-> data[[i,26]]
|>];
,
{i, 1, n}
];
{car, inputs}
]

ImportPredictions[filename_] := Module[{data, n , car},
data = Import[filename, "CSV"];
n = Length[data];
car = {};
Do[
AppendTo[car, <|
"pos"-> data[[i,1;;3]],
"vel"-> data[[i,4;;6]],
"\[Omega]"-> data[[i,7;;9]]
|>]
,
{i, 1, n}
];
car
]

End[];
EndPackage[];
