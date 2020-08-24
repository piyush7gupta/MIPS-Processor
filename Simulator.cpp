#include<bits/stdc++.h>
#include<array>
using namespace std;

string convert_to_instr(string s)
{
    int count =0;
    string s1;
    int i=0;
    while(count<32)
    {
        if(s[i]!=' ')
        {
            s1.push_back(s[i]);
            count++;
        }
        i++;
        
    }
    return s1;
}
void int_to_binary(vector<int> &v,int a,int start,int count)
{
    int i=start;
    while(a!=0)
    {
        v[i]=a%2;
        i++;
        a/=2;
    }
    while(i<start+count)
    {
        v[i]=0;
        i++;
    }
    return ;
}
void string_to_binary(vector<int>&v,string a,int start)
{
    for(int i=0;i<32;i++)
       {
           if(a[32-i-1]=='0')
           v[start+i]=0;
           else
               v[start+i]=1; 
       } 
    return;
}

void copy_vector(vector<int> v1,vector<int>&v2,int start1,int start2,int count)
{
    //copy from v1 to v2
    for(int i=0;i<count;i++)
        v2[start2+i]=v1[start1+i];

    return;
}

int binary_to_int(vector<int>v,int start,int count)
{
    int ans=0;
    for(int i=start+count-1;i>=start;i--)
    {
        ans*=2;
        ans+=v[i];
    }
    return ans;
}

void decoding_instr_control(vector<int> instr, int start,vector<int>&control)
{
    vector<int> control1(9,0);
    control=control1;
    int op=binary_to_int(instr,start+26,6);
    if(op==0)           //R-format
    {
        control[0]=1;       //regDst
        control[1]=1;       //ALUOP1
        control[2]=0;       //ALUOP2
        control[3]=0;       //ALUSrc
        control[4]=0;       //branch
        control[5]=0;       //Mem_Write
        control[6]=0;       //Mem_REad
        control[7]=0;       //Mem_to_reg
        control[8]=1;       //Reg_write
    }
    else if(op==35)          //load
    {
        control[0]=0;       //regDst
        control[1]=0;       //ALUOP1
        control[2]=0;       //ALUOP2
        control[3]=1;       //ALUSrc
        control[4]=0;       //branch
        control[5]=0;       //Mem_Write
        control[6]=1;       //Mem_REad
        control[7]=1;       //Mem_to_reg
        control[8]=1;       //Reg_write
    }
    else if(op==43)         //store
    {
        control[0]=0;       //regDst
        control[1]=0;       //ALUOP1
        control[2]=0;       //ALUOP2
        control[3]=1;       //ALUSrc
        control[4]=0;       //branch
        control[5]=1;       //Mem_Write
        control[6]=0;       //Mem_REad
        control[7]=0;       //Mem_to_reg
        control[8]=0;       //Reg_write
    }
    else if(op==4)          //branch equal
    {
        control[0]=0;       //regDst
        control[1]=0;       //ALUOP1
        control[2]=1;       //ALUOP2
        control[3]=0;       //ALUSrc
        control[4]=1;       //branch
        control[5]=0;       //Mem_Write
        control[6]=0;       //Mem_REad
        control[7]=0;       //Mem_to_reg
        control[8]=0;       //Reg_write
    }
    else
    {
        control[0]=0;       //regDst
        control[1]=0;       //ALUOP1
        control[2]=0;       //ALUOP2
        control[3]=0;       //ALUSrc
        control[4]=0;       //branch
        control[5]=0;       //Mem_Write
        control[6]=0;       //Mem_REad
        control[7]=0;       //Mem_to_reg
        control[8]=0;       //Reg_write
    }
    
}


int main()
{
    srand(time(0));
    string s;
    int a=0;
    int j=0;
    vector <string> inst;
    int stall=0;
    long reg[32],mem[4096];
    for(int i=0;i<32;i++)
    reg[i]=0;
    for(int i=0;i<4096;i++)
    mem[i]=0;

    int N=15;
    float x=0.7;
    // reg[0]=15;
     reg[1]=3;
     reg[2]=16;
     reg[3]=123;
     reg[4]=13223;
     reg[5]=6;
    reg[13]=5; 
    reg[14]=1;
    reg[15]=8;
    reg[19]=13223;
    reg[20]=133;
    reg[23]=20;
    reg[29]=4096;
    reg[31]=38;
    mem[0]=19;
    mem[1]=15;
    mem[2]=435;
    mem[3]=42;
    mem[4]=123;
    mem[20]=1213;

    vector<int> IF_ID(200,-1);         // PC(0-31) Instr(32-63)  "with least significant bit at starting" Instr_NUM(64)
    vector<int> ID_EX(220,-1);         // PC(0-31) Rs(32-63) Rt(64-95) Reg_Rs_num[96-100] Reg_Rt_num[20-16](101-105) Reg_Rd_num(15-11)[106-110] control[111-119] immediate[120-151] Instr[152-183] Instr_NUM(184) Jump[(185)] Jump_PC(186-217)
    vector<int> EX_MEM(200,-1);        // (PC+sign_extended) [0-31] ALU_output[32-63] ALU_zero[64] Write_Reg_val[65-96] Reg_dest_num[97-101] control[102-106] INSTR_NUM(107) Jump(108) JUump_PC(109-140)
    vector<int> MEM_WB(200,-1);        // Read_data[0-31]  ALU_output[32-63] Reg_Dest_num[64-68] contol[69-70] INSTR_NUM[71] Jump(72)
    
    int real_pc_counter=0;
    int b=0;
    while(getline(cin,s))
    {
        a++;
        inst.push_back(convert_to_instr(s));
        cout<<a<<endl;
    }
    int total_instr=inst.size();
    int cycle_count=0;
    int l=0;

     printf("-------------Values of register after initially ----------\n");
       for(int i=0;i<32;i++)
            printf("Value of reg at index %d is %ld\n",i,reg[i]);
        for(int i=5;i<9;i++)
            printf("Value of mem at index %d is %ld\n",i,mem[i]);


    while(b!=5)
    {
       l++;
        cycle_count++;
        string curr_inst;
        b=0;

        vector<int> IF_ID_read=IF_ID;
        vector<int> ID_EX_read=ID_EX;
        vector<int> EX_MEM_read=EX_MEM;
        vector<int> MEM_WB_read=MEM_WB;

         cout<<cycle_count<<" Stage1 ";
        if(real_pc_counter<total_instr)
            cout<<real_pc_counter;
        else
            cout<<"NO_INstruction ";
        cout<<endl;

        cout<<cycle_count<<" Stage2 ";
        if(IF_ID_read[0]!=-1)
            cout<<IF_ID_read[64];
        else
            cout<<"NO_INstruction ";
        cout<<endl;

        cout<<cycle_count<<" Stage3 ";
        if(ID_EX_read[0]!=-1)
            cout<<ID_EX_read[184];
        else
            cout<<"NO_INstruction";
        cout<<endl;

        cout<<cycle_count<<" Stage4 ";
        if(EX_MEM_read[0]!=-1)
            cout<<EX_MEM_read[107];
        else
            cout<<"NO_INstruction ";
        cout<<endl;

        cout<<cycle_count<<" Stage5 ";
        if(MEM_WB_read[0]!=-1) 
            cout<<MEM_WB_read[71];
        else
            cout<<"NO_INstruction ";

        cout<<endl;


       //STAGE 5
        if(MEM_WB_read[0]!=-1)
       {
         //  cout<<MEM_WB_read[71]<<" "<<MEM_WB_read[69]<<endl;;
           if(MEM_WB_read[69+1]==1)         //Reg_Write
           {
               int write_dest=binary_to_int(MEM_WB_read,64,5);

               if(MEM_WB_read[69]==1)           //mem_to_reg
                reg[write_dest]=binary_to_int(MEM_WB_read,0,32);
               else 
                reg[write_dest]=binary_to_int(MEM_WB_read,32,32);
           } 
       }
       else
            b++;


       //STAGE 1
       if(real_pc_counter<total_instr)
       {
            curr_inst=inst[real_pc_counter];
            int_to_binary(IF_ID,real_pc_counter,0,32);
            string_to_binary(IF_ID,curr_inst,32);
            IF_ID[64]=real_pc_counter;
            real_pc_counter++;
           
       }
       else
       {   
           IF_ID[0]=-1;
           b++;
       }


        //STAGE 2       
       if(IF_ID_read[0]!=-1)
       {
            copy_vector(IF_ID_read,ID_EX,0,0,32); //passing the PC

            int reg_rs_num=binary_to_int(IF_ID_read,53,5);
            int reg_rt_num=binary_to_int(IF_ID_read,48,5);
            int reg_rd_num=binary_to_int(IF_ID_read,43,5);
            int reg_rs=reg[reg_rs_num];
            int reg_rt=reg[reg_rt_num];
            int immediate=binary_to_int(IF_ID_read,32,16);

            int_to_binary(ID_EX,reg_rs,32,32);
            int_to_binary(ID_EX,reg_rt,64,32);

            int_to_binary(ID_EX,reg_rs_num,96,5);
            int_to_binary(ID_EX,reg_rt_num,101,5);
            int_to_binary(ID_EX,reg_rd_num,106,5);
            int_to_binary(ID_EX,immediate,120,32);

            vector<int>control;
            decoding_instr_control(IF_ID_read,32,control);
            int op=binary_to_int(IF_ID_read,32+26,6);
            if(op==2)
                {
                    ID_EX[185]=1;
                    ID_EX[186]=0;
                    ID_EX[187]=0;
                    copy_vector(IF_ID_read,ID_EX,28,214,4);
                    copy_vector(IF_ID_read,ID_EX,32,188,26);
                }
                else
                {
                    ID_EX[185]=0;
                }
            

            copy_vector(control,ID_EX,0,111,9);
            

            copy_vector(IF_ID_read,ID_EX,32,152,32);

            ID_EX[184]=IF_ID_read[64];

            int case1=0;
            
            int EX_MEM_Reg_write=EX_MEM_read[106];          //reg_write signal of ex_mem stage
            int EX_MEM_Reg_rd=binary_to_int(EX_MEM_read,97,5);
           
            int k=ID_EX_read[119];                          //reg_write signal of ID_EX stage
            int dest=0;
             if(ID_EX_read[111]==0)                         //reg_dest for ID_EX stage
                dest=binary_to_int(ID_EX_read,101,5);
            else
                dest=binary_to_int(ID_EX_read,106,5);


            int rs_num=binary_to_int(IF_ID,53,5);
            if(binary_to_int(IF_ID_read,58,6)==35)          // load instruction
            reg_rt_num=38;
            /*I have changed reg_rt_num so that in the consequent checking of the registers the reg_rt_num should not be checked 
            as in this register value is to be loaded so it will have no effext if it is same to previously loaded register*/
            
            int rt_num=binary_to_int(IF_ID,48,5);

            if((ID_EX_read[117]==1)&& ((reg_rs_num == binary_to_int(ID_EX_read,101,5)) || (reg_rt_num==binary_to_int(ID_EX_read,101,5)) ))  
            {
                if(IF_ID[0]!=-1)            // -1 means that the current instruction is the last instrution.
                real_pc_counter=binary_to_int(IF_ID,0,32);
               IF_ID=IF_ID_read;    
               vector<int>control2(9,0);
               copy_vector(control2,ID_EX,0,111,9);
            }


       }
       else
       {
           ID_EX[0]=-1;
           b++;
       }
       //STAGE 3
       if(ID_EX_read[0]!=-1)
       {
           
           EX_MEM[107]=ID_EX_read[184];
           int pc=binary_to_int(ID_EX_read,0,32);
           int immediate=binary_to_int(ID_EX_read,120,32);
           pc+=immediate;
           int_to_binary(EX_MEM,pc,0,32);

           int source1=binary_to_int(ID_EX_read,32,32);
           int source2;
           if(ID_EX_read[111+3]==0)            // to choose between rt and intermediate        
               source2=binary_to_int(ID_EX_read,64,32);
           else
               source2=binary_to_int(ID_EX_read,120,32);                //intermediate

            int write_reg_value=binary_to_int(ID_EX_read,64,32);

            int dest2=binary_to_int(MEM_WB_read,64,5);
            if((MEM_WB_read[70]==1)&&(dest2!=0)&&(dest2==binary_to_int(ID_EX_read,96,5)) )      //Forwarding data which was going to be written in stage5 
            {
                if(MEM_WB_read[69]==1)                              
                    source1=binary_to_int(MEM_WB_read,0,32);        //if the data is memory one
                else
                    source1=binary_to_int(MEM_WB_read,32,32);       //if the data is ALU one
            }
        
            if((MEM_WB_read[70]==1)&&(dest2!=0)&&(dest2==binary_to_int(ID_EX_read,101,5))  )     //Forwarding data which was going to be written in stage5 
            {
                if(ID_EX_read[114]==0 )            //if the second source is rt not intermediate
                {
                    if(MEM_WB_read[69]==1)
                        source2=binary_to_int(MEM_WB_read,0,32);        //if the data is memory one
                    else
                        source2=binary_to_int(MEM_WB_read,32,32);       //if the data is ALU one
                }

                if(MEM_WB_read[69]==1)                                 //if the data is memory one
                write_reg_value=binary_to_int(MEM_WB_read,0,32);  
                else                                                   //if the data is ALU one
                write_reg_value=binary_to_int(MEM_WB_read,32,32);  

            }


            int dest;
            dest=binary_to_int(EX_MEM_read,97,5);
            if((EX_MEM_read[106]==1) && (dest!=0)&&(dest==binary_to_int(ID_EX_read,96,5)))
            source1=binary_to_int(EX_MEM_read,32,32);

            if((EX_MEM_read[106]==1) && (dest!=0)&&(dest==binary_to_int(ID_EX_read,101,5)) )
            {
                if(ID_EX_read[114]==0)                  //if the second source is rt not intermediate
                source2=binary_to_int(EX_MEM_read,32,32);

                write_reg_value=binary_to_int(EX_MEM_read,32,32);       // ALU output forwarded

            }
           
           int alu_control=0;     //0_for add 1_for_sub  2_for_or  3_for_and  

           if(ID_EX_read[111+1]==0 && ID_EX_read[111+2]==0)
            alu_control=0;
           else if(ID_EX_read[111+1]==0 && ID_EX_read[111+2]==1)
            alu_control=1;
           else if(ID_EX_read[111+1]==1 && ID_EX_read[111+2]==0)
           {
               int a=binary_to_int(ID_EX_read,152,6);
               if(a==32)            //add
               alu_control=0;
               else if(a==34)       //sub
               alu_control=1;
               else if(a==37)       //or
               alu_control=2;
               else if(a==36)       //and
               alu_control=3;       
           }

           int alu_output;
           if(alu_control==0)
            alu_output=source1+source2;
           else if(alu_control==1)
            alu_output=source1-source2;
           else if(alu_control==2)
            alu_output=source1 | source2;
           else if(alu_control==3)
            alu_output=source1 & source2;


            int_to_binary(EX_MEM,alu_output,32,32);
            if(alu_output==0)
            EX_MEM[64]=1;
            else
            EX_MEM[64]=0;
          
            int_to_binary(EX_MEM,write_reg_value,65,32);
            if(ID_EX_read[111]==0)
               copy_vector(ID_EX_read,EX_MEM,101,97,5);
            else
               copy_vector(ID_EX_read,EX_MEM,106,97,5);
              
            
            copy_vector(ID_EX_read,EX_MEM,115,102,5);

            
            if(ID_EX_read[185]==1)
            {
               // cout<<"asdasd"<<endl;
                copy_vector(ID_EX_read,EX_MEM,185,108,33);//copying Jump balidity and its pc
            }
            
          
       }
       else
       {
           EX_MEM[0]=-1;
           b++;
       }
       
       //STAGE 4
       if(EX_MEM_read[0]!=-1)
       {


           MEM_WB[71]=EX_MEM_read[107];

          
           cout<<stall<<endl;
           if( stall==0 &&((EX_MEM_read[104]==1)|| (EX_MEM_read[103]==1)))
           {
               int random_num=rand()%1000;              //generating random number to check if it is hit or miss
                float value=(random_num*1.0)/1000;
              cout<<x<<" "<<value<<endl;
               if(x>value)
                {
                    stall=N;
               cout<<"STALLING "<<N-1<< " cycles "<<endl;
                }
           }
           if(stall>1)
           {
               if(IF_ID[0]!=-1)            // -1 means that the current instruction is the last instrution.
                real_pc_counter=binary_to_int(IF_ID,0,32);
               IF_ID=IF_ID_read;
               ID_EX=ID_EX_read;
               EX_MEM=EX_MEM_read;
               
               stall--;
               vector<int> control1(5,0);
               copy_vector(control1,EX_MEM_read,0,102,5);  

           }
           else if(stall==1)                // this if for the last cycle in which the instruction should be executed
            stall--;

           int mem_read=0;
           int mem_dest=binary_to_int(EX_MEM_read,32,32);
           if(EX_MEM_read[102+2]==1)        //Mem_Read
            mem_read=mem[mem_dest];
           if(EX_MEM_read[102+1]==1)        //Mem_Write
            mem[mem_dest]=binary_to_int(EX_MEM_read,65,32);


           int_to_binary(MEM_WB,mem_read,0,32);
           int_to_binary(MEM_WB,mem_dest,32,32);

           copy_vector(EX_MEM_read,MEM_WB,97,64,5);
           copy_vector(EX_MEM_read,MEM_WB,105,69,2);

           
       }
       else
       {
           MEM_WB[0]=-1;
           b++;
       }
       
       
        //Branching
       if(EX_MEM_read[102]==1 && EX_MEM_read[64]==1)
       {
         //  cout<<"ysaoy"<<endl;
           IF_ID[0]=-1;
           ID_EX[0]=-1;
           EX_MEM[0]=-1;
           MEM_WB[0]=-1;
           real_pc_counter=binary_to_int(EX_MEM_read,0,32);
       }
       if(EX_MEM_read[108]==1)
       {
           //cout<<"yooy"<<endl;
           IF_ID[0]=-1;
           ID_EX[0]=-1;
           ID_EX[15]=0;
           EX_MEM[108]=0;
           EX_MEM[0]=-1;
           MEM_WB[0]=-1;
           cout<<binary_to_int(EX_MEM_read,109,32)<<endl;
           real_pc_counter=binary_to_int(EX_MEM_read,109,32);
       }
         printf("-------------Values of register after %d instr %ld %ld---------\n",cycle_count,reg[1] , mem[5]);
       cout<<endl;

       
    }

     printf("-------------Values of register after %d instr ----------\n",cycle_count);
       for(int i=0;i<32;i++)
            printf("Value of reg at index %d is %ld\n",i,reg[i]);
        for(int i=5;i<9;i++)
            printf("Value of mem at index %d is %ld\n",i,mem[i]);



    cout<<endl<<"Total Cycle Count "<< cycle_count<<endl;
    return 0;
}