#include <cstring>

#include "ZiNode.h"

//
// ZiNode class
//

ZiNode::ZiNode (
    int c_rank,
    char* c_data,
    double c_raw_freq,
    double c_centile,
    char* c_fayin,
    int c_tone
    )
{
    rank = c_rank;
    memcpy(data, c_data, sizeof(data));
    raw_freq = c_raw_freq;
    centile = c_centile;
    memcpy(fayin, c_fayin, sizeof(fayin));
    tone = c_tone;
}
