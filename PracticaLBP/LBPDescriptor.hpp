#include <iostream>
#include <cstdlib>
#include <vector>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define compab_mask_inc(ptr,shift) \
{ value |= ((unsigned int)(cntr - *ptr) & 0x80000000) \
>> (31-shift); ptr++; }

class LBPDescriptor {

private:
    int* LBP8(const int*, int, int);

public:
    LBPDescriptor();
    vector<int> histogramaLBP(Mat);
};