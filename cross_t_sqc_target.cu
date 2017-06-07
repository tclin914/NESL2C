#include <stdio.h>
#include "pf/pf.h"
struct tuple_F_F
{
float a;
float b;
};

struct tuple_tuple_F_F_tuple_F_F
{
struct tuple_F_F a;
struct tuple_F_F b;
};

struct tuple_F_F;

struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F
{
struct tuple_F_F a;
struct tuple_tuple_F_F_tuple_F_F b;
};

struct tuple_tuple_F_F_tuple_F_F;

float __device__ __forceinline__ cross_product(struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F tmp1)
{
float res;
struct tuple_F_F tmp2;
struct tuple_tuple_F_F_tuple_F_F tmp3;
float t1, t2, t3, t4, t5, t6, t7;
tmp2=tmp1.a;
tmp3=tmp1.b;
t1=(tmp3.a.a-tmp2.a);
t2=(tmp3.b.b-tmp2.b);
t3=(t1*t2);
t4=(tmp3.a.b-tmp2.a);
t5=(tmp3.b.a-tmp2.b);
t6=(t4*t5);
t7=(t3-t6);
res=t7;
return res;
}

void __device__ myFunc1_deviceEntry00(int32_t mask00)
{
struct tuple_F_F tmp4, tmp5, tmp6;
struct tuple_tuple_F_F_tuple_F_F tmp7;
struct tuple_tuple_F_F_tuple_tuple_F_F_tuple_F_F tmp8;
float res;
if ((mask00&1)==1)
{
tmp4.a=1.0;
tmp4.b=1.0;
tmp5.a=5.0;
tmp5.b=2.4;
tmp6.a=4.1;
tmp6.b=4.3;
tmp7.a=tmp5;
tmp7.b=tmp6;
tmp8.a=tmp4;
tmp8.b=tmp7;
res=cross_product(tmp8);
printf("res = %f\n", res);
}
}

void __global__ __launch_bounds__(BLOCK_SIZE, (NUM_BLOCKS/NUM_SMS)) myFunc1()
{
bool mask00;
initializeContext();
mask00=((gangID()==0)&&(workerID()==0));
if (anyTrue(mask00)||getWorkAndSync(0, 0))
{
incRecurLevel(0);
myFunc1_deviceEntry00((mask00 ? 1 : 0));
getWorkAndSync(0, 0);
}
}

int main()
{
CHECK_ERR(cudaDeviceSetLimit(cudaLimitStackSize, LOCAL_STACK_SIZE));
CHECK_ERR(cudaFuncSetCacheConfig(myFunc1, cudaFuncCachePreferShared));
initializeGPUData();
myFunc1<<<NUM_BLOCKS, BLOCK_SIZE>>>();
cudaDeviceSynchronize();
return 1;
}

