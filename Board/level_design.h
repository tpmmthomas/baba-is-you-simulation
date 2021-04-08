// level design
//have dimension of [4][12][16]
const char* levels_board[4][12] = {{"________________",
															"_______FIW______",
															"________________",
															"________________",
															"_______BIY______",
															"________________",
															"____________f___",
															"________________",
															"____b___________",
															"________________",
															"___ggggggg______",
															"________________"},
														 {"BGg____ggggggg__",
														  "IIg____g_____g__",
															"YSg___Vg_b_r_g__",
															"ggg___Ig_____g__",
															"______Tg___r_g__",
															"___gggggvvvvvgg_",
															"___g__________g_",
															"___gvvv___RIP_g_",
															"___gvvv_______g_",
															"___gfvv___FIW_g_",
															"___gggggggggggg_",
															"________________"
														 },
														 {"FIW_____________",
														  "__________JIP___",
															"_______j___j____",
															"_ggggg___qqqqq__",
															"Gg___g___q___q__",
															"Ig_b_j___q_f_q__",
															"Sg___g___q___q__",
															"_ggggg___qqqqq__",
															"__j_____________",
															"______j____j____",
															"__BIY_______j___",
															"_____________QID"
														 },			
														 {"gggggggggggggggg",
														  "gggggggggggggggg",
															"________gggggggg",
															"___b____q__JF_gg",
															"__BIY___g__gIIgg",
															"________g__gDWgg",
															"____ggggg__ggggg",
															"____g___g_____gg",
															"____jjf_g_QIP_gg",
															"GISgg___g_____gg",
															"gggggggggggggggg",
															"gggggggggggggggg"
														 }
};



//index 0 corresponds to ascii 65, content being index of correct element in images.h
//For example: mapping[1] = 9, which means "B" (with ascii code 66) correspnds to index 9 (which is baba_text) in images.h
//-1 represents nothing
//Capital letter represents words
//Small letter represents objects
//Please try to map same object and its corresponding text with the same letter!! (Have to note all descrepencies)
const int mapping[62]={
-1, //A
9, //B 
-1, //C
21, //D
-1, //E
5, //F
14, //G
-1, //H
11, //I
17, //J
-1, //K
-1, //L
-1, //M
-1, //N
-1, //O
13, //P
19, //Q
16, //R
8, //S
12, //T (This is sink)
-1, //U
3, //V
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
18, //j
-1, //k
-1, //l
-1, //m
-1, //n
-1, //o
-1, //p
20, //q
15, //r
-1, //s
-1, //t
-1, //u
7, //v
-1, //w
-1, //x
-1, //y
-1, //z
-1, //{
-1, //|
-1, //}
-1 //~
};
