// level design
//have dimension of [4][12][16]
const char* levels[4][12] = {{"________________",
															"_______FIW______",
															"____________f___",
															"________________",
															"_______BIY______",
															"________________",
															"________________",
															"________________",
															"____b___________",
															"________________",
															"___ggggggg______",
															"________________"}
};

//index 0 corresponds to ascii 65, content being index of correct element in images.h
//For example: mapping[1] = 9, which means "B" (with ascii code 66) correspnds to index 9 (which is baba_text) in images.h
//-1 represents nothing
const int mapping[62]={
-1, //A
9, //B
-1, //C
-1, //D
-1, //E
5, //F
-1, //G
-1, //H
11, //I
-1, //J
-1, //K
-1, //L
-1, //M
-1, //N
-1, //O
-1, //P
-1, //Q
-1, //R
-1, //S
-1, //T
-1, //U
-1, //V
6, //W
-1, //X
10, //Y
-1, //Z
-1, //[
-1, // '\' don't use this
-1, //]
-1, //^
-1, //_
-1, //`
-1, //a
0, //b
-1, //c
-1, //d
-1, //e
2, //f
4, //g
-1, //h
-1, //i
-1, //j
-1, //k
-1, //l
-1, //m
-1, //n
-1, //o
-1, //p
-1, //q
-1, //r
-1, //s
-1, //t
-1, //u
-1, //v
-1, //w
-1, //x
-1, //y
-1, //z
-1, //{
-1, //|
-1, //}
-1 //~
};
