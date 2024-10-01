#include "RendererDefs.h"

namespace Render
{

bool operator>(const DrawQueueElmnt& queueElmnt_1, const DrawQueueElmnt& queueElmnt_2)
{
    return queueElmnt_1.distFromCamera > queueElmnt_2.distFromCamera;   
}

}