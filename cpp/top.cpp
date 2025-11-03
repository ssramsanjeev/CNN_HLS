#include<ap_int.h>
#define iint ap_uint<8>

void multi(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2,par3;
    par1=0;
    par2=0;
    par3=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<3;i++){
        par1+=in[i][0]*mul[i][0];
        par2+=in[i][1]*mul[i][1];
        par3+=in[i][2]*mul[i][2];
    }
    fi=par1+par2+par3;
}

void col_left_mult(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<3;i++){
        par1+=in[i][0]*mul[i][1];
        par2+=in[i][1]*mul[i][2];
    }
    fi=par1+par2;
}

void col_right_mult(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<3;i++){
        par1+=in[i][1]*mul[i][0];
        par2+=in[i][2]*mul[i][1];
    }
    fi=par1+par2;
}

void row_top_mult(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2,par3;
    par1=0;
    par2=0;
    par3=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i][0]*mul[i+1][0];
        par2+=in[i][1]*mul[i+1][1];
        par3+=in[i][2]*mul[i+1][2];
    }
    fi=par1+par2+par3;
}

void row_bottom_mult(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2,par3;
    par1=0;
    par2=0;
    par3=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i+1][0]*mul[i][0];
        par2+=in[i+1][1]*mul[i][1];
        par3+=in[i+1][2]*mul[i][2];
    }
    fi=par1+par2+par3;
}

void cor_mult_tl(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i][0]*mul[i+1][1];
        par2+=in[i][1]*mul[i+1][2];
    }
    fi=par1+par2;
}

void cor_mult_tr(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i][1]*mul[i+1][0];
        par2+=in[i][2]*mul[i+1][1];
    }
    fi=par1+par2;
}

void cor_mult_bl(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i+1][0]*mul[i][1];
        par2+=in[i+1][1]*mul[i][2];
    }
    fi=par1+par2;
}

void cor_mult_br(iint in[3][3],iint mul[3][3],iint& fi){
    iint par1,par2;
    par1=0;
    par2=0;
    //#pragma HLS PIPELINE II=3
    for(int i=0;i<2;i++){
        par1+=in[i+1][1]*mul[i][0];
        par2+=in[i+1][2]*mul[i][1];
    }
    fi=par1+par2;
}
#define NR_0 64
#define NC_0 64

static iint demo[NR_0][NC_0][3][3];


template<int NR,int NC>
void img_sharp(iint A[NR][NC],iint B[3][3],iint out[NR][NC]){
    //#pragma HLS ARRAY_PARTITION variable=A dim=2 type=complete
    //#pragma HLS ARRAY_PARTITION variable=B dim=2 type=complete

    //int demo[NR-2][NC-2][3][3];

    //#pragma HLS bind_storage variable=demo type=RAM_LUTRAM
    for(int i=0;i<NR-2;i++){
        for(int j=0;j<NC-2;j++){
            for(int x=0;x<3;x++){
                for(int y=0;y<3;y++){
                    demo[i][j][x][y]=A[i+x][j+y];
                }
            }
        }
    }
    
    //#pragma HLS ARRAY_PARTITION variable=demo dim=4 type=complete

    for(int i=0;i<NR-2;i++){
       // #pragma HLS PIPELINE
        for(int j=0;j<NC-2;j++){
            //#pragma HLS UNROLL factor=10
            multi(demo[i][j],B,out[i+1][j+1]);
        }
    }

    for(int i=0;i<NR-2;i++){
        col_left_mult(demo[i][0],B,out[i+1][0]);        
    }

    for(int i=0;i<NR-2;i++){
        col_right_mult(demo[i][NC-3],B,out[i+1][NC-1]);        
    }

    for(int j=0;j<NC-2;j++){
        row_top_mult(demo[0][j],B,out[0][j+1]);
    }

    for(int j=0;j<NC-2;j++){
        row_bottom_mult(demo[NR-3][j],B,out[NR-1][j+1]);
    }
    

    cor_mult_tl(demo[0][0],B,out[0][0]);
    cor_mult_tr(demo[0][NC-3],B,out[0][NC-1]);
    cor_mult_bl(demo[NR-3][0],B,out[NR-1][0]);
    cor_mult_br(demo[NR-3][NC-3],B,out[NR-1][NC-1]);

}

#define NR_1 NR_0/2
#define NC_1 NC_0/2


#define NR_2 NR_1/2
#define NC_2 NC_1/2



#define NR_3 NR_2/2
#define NC_3 NC_2/2


#define NR_4 NR_3/2
#define NC_4 NC_3/2



#define NR_5 NR_4/2
#define NC_5 NC_4/2

void max(iint i,iint j, iint k, iint l, iint& out){
    if(i>=j && i>=k && i>=l){
        out=i;
    }
    else if(j>=i && j>=k && j>=l){
        out=j;
    }
    else if(k>=i && k>=j && k>=l){
        out=k;
    }
    else{
        out=l;
    }

}

void convol(iint A[NR_0][NC_0], iint out[NR_5][NC_5]){


    //#pragma HLS ARRAY_PARTITION variable=A dim=2 type=complete
    #pragma HLS ALLOCATION function instances=max limit=1
    #pragma HLS ALLOCATION function instances=multi limit=5
    iint sobel_x[3][3] = {
    { 1,  0,  -1 },
    { 2,  0,  -2 },
    { 1,  0,  -1 }
    };
    iint sobel_y[3][3] = {
        { 1, 2, 1 },
        {  0,  0,  0 },
        { -1, -2, -1 }
    };
    //#pragma HLS ARRAY_PARTITION variable=sobel_x dim=2 type=complete
    //#pragma HLS ARRAY_PARTITION variable=sobel_y dim=2 type=complete
    iint inter_1[NR_0][NC_0];
    iint inter_2[NR_0][NC_0];
    img_sharp<NR_0,NC_0>(A, sobel_x,inter_1);
    img_sharp<NR_0,NC_0>(inter_1,sobel_y,inter_2);
    for(int i=0;i<NR_0;i++){
        for(int j=0;j<NC_0;j++){
            if(inter_2[i][j]<0){
                inter_2[i][j]=0;
            }
        }
    }
    iint inter_3[NR_1][NC_1];
    for(int i=0;i<NR_0;i=i+2){
        for(int j=0;j<NC_0;j=j+2){
            max(inter_2[i][j],inter_2[i][j+1],inter_2[i+1][j],inter_2[i+1][j+1],inter_3[i/2][j/2]);
        }
    }

    iint laplacian[3][3] = {
        {  0,  1,  0 },
        {  1, -4,  1 },
        {  0,  1,  0 }
    };
    //#pragma HLS ARRAY_PARTITION variable=laplacian dim=2 type=complete
    iint inter_4[NR_1][NC_1];
    img_sharp<NR_1,NC_1>(inter_3,laplacian,inter_4);
    for(int i=0;i<NR_1;i++){
        for(int j=0;j<NC_1;j++){
            if(inter_4[i][j]<0){
                inter_4[i][j]=0;
            }
        }
    }
    iint inter_5[NR_2][NC_2];
    for(int i=0;i<NR_1;i=i+2){
        for(int j=0;j<NC_1;j=j+2){
            max(inter_4[i][j],inter_4[i][j+1],inter_4[i+1][j],inter_4[i+1][j+1],inter_5[i/2][j/2]);
        }
    }

    iint gaussian[3][3] = {
        { 1, 2, 1 },
        { 2, 4, 2 },
        { 1, 2, 1 }
    }; // Normalize by /16
    //#pragma HLS ARRAY_PARTITION variable=gaussian dim=2 type=complete
    iint inter_6[NR_2][NC_2];
    img_sharp<NR_2,NC_2>(inter_5,gaussian,inter_6);
    for(int i=0;i<NR_2;i++){
        for(int j=0;j<NC_2;j++){
            if(inter_6[i][j]<0){
                inter_6[i][j]=0;
            }
        }
    }
    iint inter_7[NR_3][NC_3];
    for(int i=0;i<NR_2;i=i+2){
        for(int j=0;j<NC_2;j=j+2){
            max(inter_6[i][j],inter_6[i][j+1],inter_6[i+1][j],inter_6[i+1][j+1],inter_7[i/2][j/2]);
        }
    }

    iint dog[3][3] = {
        { -1, -1, -1 },
        { -1,  8, -1 },
        { -1, -1, -1 }
    };
    //#pragma HLS ARRAY_PARTITION variable=dog dim=2 type=complete
    iint inter_8[NR_3][NC_3];
    img_sharp<NR_3,NC_3>(inter_7,dog,inter_8);
    for(int i=0;i<NR_3;i++){
        for(int j=0;j<NC_3;j++){
            if(inter_8[i][j]<0){
                inter_8[i][j]=0;
            }
        }
    }
    iint inter_9[NR_4][NC_4];
    for(int i=0;i<NR_3;i=i+2){
        for(int j=0;j<NC_3;j=j+2){
            max(inter_8[i][j],inter_8[i][j+1],inter_8[i+1][j],inter_8[i+1][j+1],inter_9[i/2][j/2]);
        }
    }

    iint sharpen[3][3] = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
    };
    //#pragma HLS ARRAY_PARTITION variable=sharpen dim=2 type=complete
    iint inter_10[NR_4][NC_4];
    img_sharp<NR_4,NC_4>(inter_9,sharpen,inter_10);
    for(int i=0;i<NR_4;i++){
        for(int j=0;j<NC_4;j++){
            if(inter_10[i][j]<0){
                inter_10[i][j]=0;
            }
        }
    }
    //int inter_11[NR_5][NC_5];
    for(int i=0;i<NR_4;i=i+2){
        for(int j=0;j<NC_4;j=j+2){
            max(inter_10[i][j],inter_10[i][j+1],inter_10[i+1][j],inter_10[i+1][j+1],out[i/2][j/2]);
        }
    }


}

#define IMG_ROWS 256
#define IMG_COLS 256
#define TILE_ROWS 64
#define TILE_COLS 64

void split_into_tiles(ap_uint<8> A[IMG_ROWS][IMG_COLS],
                      ap_uint<8> tile0[TILE_ROWS][TILE_COLS],
                      ap_uint<8> tile1[TILE_ROWS][TILE_COLS],
                      ap_uint<8> tile2[TILE_ROWS][TILE_COLS],
                      ap_uint<8> tile3[TILE_ROWS][TILE_COLS]) {
    #pragma HLS INLINE off
    // Top-left tile (0,0)
    for (int i = 0; i < TILE_ROWS; i++) {
        for (int j = 0; j < TILE_COLS; j++) {
            //#pragma HLS PIPELINE II=1
            tile0[i][j] = A[i][j];
        }
    }

    // Top-right tile (0,64)
    for (int i = 0; i < TILE_ROWS; i++) {
        for (int j = 0; j < TILE_COLS; j++) {
            //#pragma HLS PIPELINE II=1
            tile1[i][j] = A[i][j + TILE_COLS];
        }
    }

    // Bottom-left tile (64,0)
    for (int i = 0; i < TILE_ROWS; i++) {
        for (int j = 0; j < TILE_COLS; j++) {
            //#pragma HLS PIPELINE II=1
            tile2[i][j] = A[i + TILE_ROWS][j];
        }
    }

    // Bottom-right tile (64,64)
    for (int i = 0; i < TILE_ROWS; i++) {
        for (int j = 0; j < TILE_COLS; j++) {
            //#pragma HLS PIPELINE II=1
            tile3[i][j] = A[i + TILE_ROWS][j + TILE_COLS];
        }
    }
}

void real_top(ap_uint<8> A[IMG_ROWS][IMG_COLS],ap_uint<8>out[14]){
    #pragma HLS ALLOCATION function instances=convol limit=1
    ap_uint<8> tile0[TILE_ROWS][TILE_COLS];
    ap_uint<8> tile1[TILE_ROWS][TILE_COLS];
    ap_uint<8> tile2[TILE_ROWS][TILE_COLS];
    ap_uint<8> tile3[TILE_ROWS][TILE_COLS];
    split_into_tiles(A,tile0,tile1,tile2,tile3);
    iint out0[NR_5][NC_5];
    iint out1[NR_5][NC_5];
    iint out2[NR_5][NC_5];
    iint out3[NR_5][NC_5];
    convol(tile0,out0);
    convol(tile1,out1);
    convol(tile2,out2);
    convol(tile3,out3);

}