(* ::Package:: *)

(* ::Input::Initialization:: *)
BeginPackage["RLBotMisc`"];

EulerAngleRotation::usage = "convert from {pitch, yaw, roll} to 3x3 orientation matrix";
AxisAngleRotation::usage = "create a rotation matrix, given an axis of rotation (with angle equal to axis magnitude)";
AntisymmetricAxis::usage = "extract the vector, \[Omega], from an antisymmetric matrix, A, such that A.x = Cross[\[Omega], x]";
RotationAxis::usage = "extract the axis-angle vector representation of the given rotation matrix";
RandomRotation::usage = "create a random 3x3 special orthogonal matrix";
\[CapitalOmega]::usage = "create the matrix that is equivalent to taking the cross product with the given vector";
AngleBetween::usage = "return the minimum angle between two rotation matrices";
solvePWL::usage = "find solution minimum norm solution to a x + b |x| + c == 0, for -1 <= x <= 1";

Begin["`Private`"];

EulerAngleRotation=Compile[{{pyr, _Real, 1}}, Module[{
CP = Cos[pyr[[1]]],
	SP = Sin[pyr[[1]]],
	CY = Cos[pyr[[2]]],
	SY = Sin[pyr[[2]]],
	CR = Cos[pyr[[3]]],
	SR = Sin[pyr[[3]]]},
	\!\(\*
TagBox[
RowBox[{"(", GridBox[{
{
RowBox[{"CP", " ", "CY"}], 
RowBox[{
RowBox[{"CY", " ", "SP", " ", "SR"}], "-", 
RowBox[{"CR", " ", "SY"}]}], 
RowBox[{
RowBox[{
RowBox[{"-", "CR"}], " ", "CY", " ", "SP"}], "-", 
RowBox[{"SR", " ", "SY"}]}]},
{
RowBox[{"CP", " ", "SY"}], 
RowBox[{
RowBox[{"CR", " ", "CY"}], "+", 
RowBox[{"SP", " ", "SR", " ", "SY"}]}], 
RowBox[{
RowBox[{"CY", " ", "SR"}], "-", 
RowBox[{"CR", " ", "SP", " ", "SY"}]}]},
{"SP", 
RowBox[{
RowBox[{"-", "CP"}], " ", "SR"}], 
RowBox[{"CP", " ", "CR"}]}
},
GridBoxAlignment->{"Columns" -> {{Center}}, "ColumnsIndexed" -> {}, "Rows" -> {{Baseline}}, "RowsIndexed" -> {}, "Items" -> {}, "ItemsIndexed" -> {}},
GridBoxSpacings->{"Columns" -> {Offset[0.27999999999999997`], {Offset[0.7]}, Offset[0.27999999999999997`]}, "ColumnsIndexed" -> {}, "Rows" -> {Offset[0.2], {Offset[0.4]}, Offset[0.2]}, "RowsIndexed" -> {}, "Items" -> {}, "ItemsIndexed" -> {}}], ")"}],
Function[BoxForm`e$, MatrixForm[BoxForm`e$]]]\)
	]
];

AxisAngleRotation=Compile[{{\[Omega], _Real, 1}}, 
Module[{\[Theta], axis,K},
\[Theta] = Norm[\[Omega]];
If[\[Theta] == 0, 
({
 {1, 0, 0},
 {0, 1, 0},
 {0, 0, 1}
})
,
axis = Normalize[\[Omega]];
K = ({
 {0, -axis[[3]], axis[[2]]},
 {axis[[3]], 0, -axis[[1]]},
 {-axis[[2]], axis[[1]], 0}
});
({
 {1, 0, 0},
 {0, 1, 0},
 {0, 0, 1}
}) + Sin[\[Theta]] K + (1.0 - Cos[\[Theta]]) K.K
]
]
];

AntisymmetricAxis[A_] := {-A[[2,3]], A[[1,3]], -A[[1,2]]};

RotationAxis[\[Theta]_] := Re[AntisymmetricAxis[MatrixLog[\[Theta]]]];

RandomRotation[] := Module[{R},
R = Orthogonalize[RandomReal[{-1, 1}, {3,3}]];
R *= Det[R];
R
]

\[CapitalOmega][q_] := ({
 {0, -q[[3]], q[[2]]},
 {q[[3]], 0, -q[[1]]},
 {-q[[2]], q[[1]], 0}
})

AngleBetween[\[Theta]1_, \[Theta]2_] := ArcCos[(Tr[\[Theta]1.Transpose[\[Theta]2]] - 1)/2]
solvePWL[a_, b_, c_] := Module[{
xP = If[Abs[a+b] < 10^-10, -1, c/(a+b)],
xM = If[Abs[a-b] < 10^-10, 1, c/(a-b)]
},
If[0 <= xP && xM <= 0,
Return[If[Abs[xP] < Abs[xM],
Clip[xP, {0, 1}],
Clip[xM, {-1, 0}]
]]
];
If[0 <= xP, Return[Clip[xP, {0, 1}]]];
If[xM <= 0, Return[Clip[xM, {-1, 0}]]];
0
]

End[];
EndPackage[];

