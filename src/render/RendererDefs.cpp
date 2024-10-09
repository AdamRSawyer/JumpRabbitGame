#include "RendererDefs.h"

namespace Render
{

bool operator>(const DrawQueueElmnt& queueElmnt_1, const DrawQueueElmnt& queueElmnt_2)
{
    return queueElmnt_1.distFromCamera > queueElmnt_2.distFromCamera;   
}

bool operator<(const CameraMoveElmnt& camMvElmnt_1, const CameraMoveElmnt& camMvElmnt_2)
{
    return camMvElmnt_1.deadline < camMvElmnt_2.deadline;
}

bool operator>(const CameraMoveElmnt& camMvElmnt_1, const CameraMoveElmnt& camMvElmnt_2)
{
    return !(camMvElmnt_1 < camMvElmnt_2);
}

}