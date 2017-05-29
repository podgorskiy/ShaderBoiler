// Copyright 2017 Stanislav Pidhorskyi. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#define REPEAT_WITH_ID_AND_COMMA(X, C) REPEAT_IC##C(X)
#define REPEAT_IC1(X) X##0
#define REPEAT_IC2(X) REPEAT_IC1(X), X##1
#define REPEAT_IC3(X) REPEAT_IC2(X), X##2
#define REPEAT_IC4(X) REPEAT_IC3(X), X##3
#define REPEAT_IC5(X) REPEAT_IC4(X), X##4
#define REPEAT_IC6(X) REPEAT_IC5(X), X##5
#define REPEAT_IC7(X) REPEAT_IC6(X), X##6
#define REPEAT_IC8(X) REPEAT_IC7(X), X##7
#define REPEAT_IC9(X) REPEAT_IC8(X), X##8
#define REPEAT_IC10(X) REPEAT_IC9(X), X##9
#define REPEAT_IC11(X) REPEAT_IC10(X), X##10
#define REPEAT_IC12(X) REPEAT_IC11(X), X##11
#define REPEAT_IC13(X) REPEAT_IC12(X), X##12
#define REPEAT_IC14(X) REPEAT_IC13(X), X##13
#define REPEAT_IC15(X) REPEAT_IC14(X), X##14
#define REPEAT_IC16(X) REPEAT_IC15(X), X##15

#define REPEAT_ASSIGNMENT_WITH_ID(X1, X2, X3, C) REPEAT_AWID##C(X1, X2, X3)
#define REPEAT_AWID1(X1, X2, X3) X1 0 X2 = X3##0;
#define REPEAT_AWID2(X1, X2, X3) REPEAT_AWID1(X1, X2, X3)  X1 1 X2 = X3##1;
#define REPEAT_AWID3(X1, X2, X3) REPEAT_AWID2(X1, X2, X3)  X1 2 X2 = X3##2;
#define REPEAT_AWID4(X1, X2, X3) REPEAT_AWID3(X1, X2, X3)  X1 3 X2 = X3##3;
#define REPEAT_AWID5(X1, X2, X3) REPEAT_AWID4(X1, X2, X3)  X1 4 X2 = X3##4;
#define REPEAT_AWID6(X1, X2, X3) REPEAT_AWID5(X1, X2, X3)  X1 5 X2 = X3##5;
#define REPEAT_AWID7(X1, X2, X3) REPEAT_AWID6(X1, X2, X3)  X1 6 X2 = X3##6;
#define REPEAT_AWID8(X1, X2, X3) REPEAT_AWID7(X1, X2, X3)  X1 7 X2 = X3##7;
#define REPEAT_AWID9(X1, X2, X3) REPEAT_AWID8(X1, X2, X3)  X1 8 X2 = X3##8;
#define REPEAT_AWID10(X1, X2, X3) REPEAT_AWID9(X1, X2, X3)  X1 9 X2 = X3##9;
#define REPEAT_AWID11(X1, X2, X3) REPEAT_AWID10(X1, X2, X3)  X1 10 X2 = X3##10;
#define REPEAT_AWID12(X1, X2, X3) REPEAT_AWID11(X1, X2, X3)  X1 11 X2 = X3##11;
#define REPEAT_AWID13(X1, X2, X3) REPEAT_AWID12(X1, X2, X3)  X1 12 X2 = X3##12;
#define REPEAT_AWID14(X1, X2, X3) REPEAT_AWID13(X1, X2, X3)  X1 13 X2 = X3##13;
#define REPEAT_AWID15(X1, X2, X3) REPEAT_AWID14(X1, X2, X3)  X1 14 X2 = X3##14;
#define REPEAT_AWID16(X1, X2, X3) REPEAT_AWID15(X1, X2, X3)  X1 15 X2 = X3##15;

#define REPEAT_ASSIGNMENT(X1, X2, X3, C) REPEAT_A##C(X1, X2, X3)
#define REPEAT_A1(X1, X2, X3) X1 0 X2 = X3;
#define REPEAT_A2(X1, X2, X3) REPEAT_A1(X1, X2, X3)  X1 1 X2 = X3;
#define REPEAT_A3(X1, X2, X3) REPEAT_A2(X1, X2, X3)  X1 2 X2 = X3;
#define REPEAT_A4(X1, X2, X3) REPEAT_A3(X1, X2, X3)  X1 3 X2 = X3;
#define REPEAT_A5(X1, X2, X3) REPEAT_A4(X1, X2, X3)  X1 4 X2 = X3;
#define REPEAT_A6(X1, X2, X3) REPEAT_A5(X1, X2, X3)  X1 5 X2 = X3;
#define REPEAT_A7(X1, X2, X3) REPEAT_A6(X1, X2, X3)  X1 6 X2 = X3;
#define REPEAT_A8(X1, X2, X3) REPEAT_A7(X1, X2, X3)  X1 7 X2 = X3;
#define REPEAT_A9(X1, X2, X3) REPEAT_A8(X1, X2, X3)  X1 8 X2 = X3;
#define REPEAT_A10(X1, X2, X3) REPEAT_A9(X1, X2, X3)  X1 9 X2 = X3;
#define REPEAT_A11(X1, X2, X3) REPEAT_A10(X1, X2, X3)  X1 10 X2 = X3;
#define REPEAT_A12(X1, X2, X3) REPEAT_A11(X1, X2, X3)  X1 11 X2 = X3;
#define REPEAT_A13(X1, X2, X3) REPEAT_A12(X1, X2, X3)  X1 12 X2 = X3;
#define REPEAT_A14(X1, X2, X3) REPEAT_A13(X1, X2, X3)  X1 13 X2 = X3;
#define REPEAT_A15(X1, X2, X3) REPEAT_A14(X1, X2, X3)  X1 14 X2 = X3;
#define REPEAT_A16(X1, X2, X3) REPEAT_A15(X1, X2, X3)  X1 15 X2 = X3;
	
#define MULL(X, Y) MULL##X##Y
#define MULL11 1
#define MULL12 2
#define MULL13 3
#define MULL14 4
#define MULL21 2
#define MULL22 4
#define MULL23 6
#define MULL24 8
#define MULL31 3
#define MULL32 6
#define MULL33 9
#define MULL34 12
#define MULL41 4
#define MULL42 8
#define MULL43 12
#define MULL44 16