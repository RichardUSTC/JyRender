#include "DeviceBuffer.h"

uint8_t GetElementUsageSize(ElementUsage usage)
{
    switch (usage)
    {
    case ElementUsage::Position:
    case ElementUsage::Normal:
    case ElementUsage::Tangent:
        {
            return 3 * sizeof(float);
        }
    case ElementUsage::UV0:
    case ElementUsage::UV1:
    case ElementUsage::UV2:
    case ElementUsage::UV3:
    case ElementUsage::UV4:
    case ElementUsage::UV5:
    case ElementUsage::UV6:
    case ElementUsage::UV7:
        {
            return 2 * sizeof(float);
        }

    case ElementUsage::VertexColor:
        {
            return sizeof(uint32_t);
        }
    case ElementUsage::BoneIndices:
        {
            return MAX_BONE_PER_VERTEX * BONE_INDEX_SIZE;
        }
    case ElementUsage::BoneWeights:
        {
            return MAX_BONE_PER_VERTEX * BONE_WEIGHT_SIZE;
        }
    default:
        {
            LOG_ERROR("Device", "Element usage {0} is not handled", usage);
        }
    }
    return 0;
}

void VertexFormat::PushElement(ElementUsage usage)
{
    mElements.push_back(usage);
    mStride += GetElementUsageSize(usage);
}
