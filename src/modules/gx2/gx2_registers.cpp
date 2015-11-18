#include "gpu/pm4.h"
#include "gpu/pm4_writer.h"
#include "gx2_registers.h"

void
GX2SetAAMask(uint8_t upperLeft,
             uint8_t upperRight,
             uint8_t lowerLeft,
             uint8_t lowerRight)
{
   GX2AAMaskReg reg;
   GX2InitAAMaskReg(&reg,
                    upperLeft,
                    upperRight,
                    lowerLeft,
                    lowerRight);
   GX2SetAAMaskReg(&reg);
}

void
GX2InitAAMaskReg(GX2AAMaskReg *reg,
                 uint8_t upperLeft,
                 uint8_t upperRight,
                 uint8_t lowerLeft,
                 uint8_t lowerRight)
{
   auto pa_sc_aa_mask = reg->pa_sc_aa_mask.value();
   pa_sc_aa_mask.AA_MASK_ULC = upperLeft;
   pa_sc_aa_mask.AA_MASK_URC = upperRight;
   pa_sc_aa_mask.AA_MASK_LLC = lowerLeft;
   pa_sc_aa_mask.AA_MASK_LRC = lowerRight;
   reg->pa_sc_aa_mask = pa_sc_aa_mask;
}

void
GX2GetAAMaskReg(GX2AAMaskReg *reg,
                be_val<uint8_t> *upperLeft,
                be_val<uint8_t> *upperRight,
                be_val<uint8_t> *lowerLeft,
                be_val<uint8_t> *lowerRight)
{
   auto pa_sc_aa_mask = reg->pa_sc_aa_mask.value();
   *upperLeft = pa_sc_aa_mask.AA_MASK_ULC;
   *upperRight = pa_sc_aa_mask.AA_MASK_URC;
   *lowerLeft = pa_sc_aa_mask.AA_MASK_LLC;
   *lowerRight = pa_sc_aa_mask.AA_MASK_LRC;
}

void
GX2SetAAMaskReg(GX2AAMaskReg *reg)
{
   auto pa_sc_aa_mask = reg->pa_sc_aa_mask.value();
   pm4::write(pm4::SetContextReg { latte::Register::PA_SC_AA_MASK, pa_sc_aa_mask.value });
}

void
GX2SetAlphaTest(BOOL alphaTest,
                GX2CompareFunction::Value func,
                float ref)
{
   GX2AlphaTestReg reg;
   GX2InitAlphaTestReg(&reg, alphaTest, func, ref);
   GX2SetAlphaTestReg(&reg);
}

void
GX2InitAlphaTestReg(GX2AlphaTestReg *reg,
                    BOOL alphaTest,
                    GX2CompareFunction::Value func,
                    float ref)
{
   auto sx_alpha_ref = reg->sx_alpha_ref.value();
   auto sx_alpha_test_control = reg->sx_alpha_test_control.value();

   sx_alpha_test_control.ALPHA_TEST_ENABLE = alphaTest;
   sx_alpha_test_control.ALPHA_FUNC = static_cast<latte::REF_FUNC>(func);
   sx_alpha_ref.ALPHA_REF = ref;

   reg->sx_alpha_ref = sx_alpha_ref;
   reg->sx_alpha_test_control = sx_alpha_test_control;
}

void
GX2GetAlphaTestReg(const GX2AlphaTestReg *reg,
                   be_val<BOOL> *alphaTest,
                   be_val<GX2CompareFunction::Value> *func,
                   be_val<float> *ref)
{
   auto sx_alpha_ref = reg->sx_alpha_ref.value();
   auto sx_alpha_test_control = reg->sx_alpha_test_control.value();

   *alphaTest = sx_alpha_test_control.ALPHA_TEST_ENABLE;
   *func = static_cast<GX2CompareFunction::Value>(sx_alpha_test_control.ALPHA_FUNC);
   *ref = sx_alpha_ref.ALPHA_REF;
}

void
GX2SetAlphaTestReg(GX2AlphaTestReg *reg)
{
   auto sx_alpha_test_control = reg->sx_alpha_test_control.value();
   pm4::write(pm4::SetContextReg { latte::Register::SX_ALPHA_TEST_CONTROL, sx_alpha_test_control.value });

   auto sx_alpha_ref = reg->sx_alpha_ref.value();
   pm4::write(pm4::SetContextReg { latte::Register::SX_ALPHA_REF, sx_alpha_ref.value });
}

void
GX2SetBlendConstantColor(float red,
                         float green,
                         float blue,
                         float alpha)
{
   GX2BlendConstantColorReg reg;
   GX2InitBlendConstantColorReg(&reg, red, green, blue, alpha);
   GX2SetBlendConstantColorReg(&reg);
}

void
GX2InitBlendConstantColorReg(GX2BlendConstantColorReg *reg,
                             float red,
                             float green,
                             float blue,
                             float alpha)
{
   reg->red = red;
   reg->green = green;
   reg->blue = blue;
   reg->alpha = alpha;
}

void
GX2GetBlendConstantColorReg(GX2BlendConstantColorReg *reg,
                            be_val<float> *red,
                            be_val<float> *green,
                            be_val<float> *blue,
                            be_val<float> *alpha)
{
   *red = reg->red;
   *green = reg->green;
   *blue = reg->blue;
   *alpha = reg->alpha;
}

void
GX2SetBlendConstantColorReg(GX2BlendConstantColorReg *reg)
{
   float colors[] = { reg->red, reg->green, reg->blue, reg->alpha };
   auto values = reinterpret_cast<uint32_t *>(colors);
   pm4::write(pm4::SetContextRegs { latte::Register::CB_BLEND_RED, { values, 4 } });
}

void
GX2SetBlendControl(GX2RenderTarget::Value target,
                   GX2BlendMode::Value colorSrcBlend,
                   GX2BlendMode::Value colorDstBlend,
                   GX2BlendCombineMode::Value colorCombine,
                   BOOL useAlphaBlend,
                   GX2BlendMode::Value alphaSrcBlend,
                   GX2BlendMode::Value alphaDstBlend,
                   GX2BlendCombineMode::Value alphaCombine)
{
   GX2BlendControlReg reg;
   GX2InitBlendControlReg(&reg,
                          target,
                          colorSrcBlend,
                          colorDstBlend,
                          colorCombine,
                          useAlphaBlend,
                          alphaSrcBlend,
                          alphaDstBlend,
                          alphaCombine);
   GX2SetBlendControlReg(&reg);
}

void
GX2InitBlendControlReg(GX2BlendControlReg *reg,
                       GX2RenderTarget::Value target,
                       GX2BlendMode::Value colorSrcBlend,
                       GX2BlendMode::Value colorDstBlend,
                       GX2BlendCombineMode::Value colorCombine,
                       BOOL useAlphaBlend,
                       GX2BlendMode::Value alphaSrcBlend,
                       GX2BlendMode::Value alphaDstBlend,
                       GX2BlendCombineMode::Value alphaCombine)
{
   auto cb_blend_control = reg->cb_blend_control.value();
   reg->target = target;
   cb_blend_control.COLOR_SRCBLEND = static_cast<latte::CB_BLEND_FUNC>(colorSrcBlend);
   cb_blend_control.COLOR_DESTBLEND = static_cast<latte::CB_BLEND_FUNC>(colorDstBlend);
   cb_blend_control.COLOR_COMB_FCN = static_cast<latte::CB_COMB_FUNC>(colorCombine);
   cb_blend_control.SEPARATE_ALPHA_BLEND = useAlphaBlend;
   cb_blend_control.ALPHA_SRCBLEND = static_cast<latte::CB_BLEND_FUNC>(alphaSrcBlend);
   cb_blend_control.ALPHA_DESTBLEND = static_cast<latte::CB_BLEND_FUNC>(alphaDstBlend);
   cb_blend_control.ALPHA_COMB_FCN = static_cast<latte::CB_COMB_FUNC>(alphaCombine);
   reg->cb_blend_control = cb_blend_control;
}

void
GX2GetBlendControlReg(GX2BlendControlReg *reg,
                      be_val<GX2RenderTarget::Value> *target,
                      be_val<GX2BlendMode::Value> *colorSrcBlend,
                      be_val<GX2BlendMode::Value> *colorDstBlend,
                      be_val<GX2BlendCombineMode::Value> *colorCombine,
                      be_val<BOOL> *useAlphaBlend,
                      be_val<GX2BlendMode::Value> *alphaSrcBlend,
                      be_val<GX2BlendMode::Value> *alphaDstBlend,
                      be_val<GX2BlendCombineMode::Value> *alphaCombine)
{
   *target = reg->target;

   auto cb_blend_control = reg->cb_blend_control.value();
   *colorSrcBlend = static_cast<GX2BlendMode::Value>(cb_blend_control.COLOR_SRCBLEND);
   *colorDstBlend = static_cast<GX2BlendMode::Value>(cb_blend_control.COLOR_DESTBLEND);
   *colorCombine = static_cast<GX2BlendCombineMode::Value>(cb_blend_control.COLOR_COMB_FCN);
   *useAlphaBlend = cb_blend_control.SEPARATE_ALPHA_BLEND;
   *alphaSrcBlend = static_cast<GX2BlendMode::Value>(cb_blend_control.ALPHA_SRCBLEND);
   *alphaDstBlend = static_cast<GX2BlendMode::Value>(cb_blend_control.ALPHA_DESTBLEND);
   *alphaCombine = static_cast<GX2BlendCombineMode::Value>(cb_blend_control.ALPHA_COMB_FCN);
}

void
GX2SetBlendControlReg(GX2BlendControlReg *reg)
{
   auto cb_blend_control = reg->cb_blend_control.value();
   auto id = static_cast<latte::Register::Value>(latte::Register::CB_BLEND0_CONTROL + reg->target);
   pm4::write(pm4::SetContextReg { id, cb_blend_control.value });
}

void
GX2SetColorControl(GX2LogicOp::Value rop3,
                   uint8_t targetBlendEnable,
                   BOOL multiWriteEnable,
                   BOOL colorWriteEnable)
{
   GX2ColorControlReg reg;
   GX2InitColorControlReg(&reg,
                          rop3,
                          targetBlendEnable,
                          multiWriteEnable,
                          colorWriteEnable);
   GX2SetColorControlReg(&reg);
}

void
GX2InitColorControlReg(GX2ColorControlReg *reg,
                       GX2LogicOp::Value rop3,
                       uint8_t targetBlendEnable,
                       BOOL multiWriteEnable,
                       BOOL colorWriteEnable)
{
   auto cb_color_control = reg->cb_color_control.value();
   cb_color_control.ROP3 = rop3;
   cb_color_control.TARGET_BLEND_ENABLE = targetBlendEnable;
   cb_color_control.MULTIWRITE_ENABLE = multiWriteEnable;

   if (colorWriteEnable) {
      cb_color_control.SPECIAL_OP = latte::CB_SPECIAL_NORMAL;
   } else {
      cb_color_control.SPECIAL_OP = latte::CB_SPECIAL_DISABLE;
   }

   reg->cb_color_control = cb_color_control;
}

void
GX2GetColorControlReg(GX2ColorControlReg *reg,
                      be_val<GX2LogicOp::Value> *rop3,
                      be_val<uint8_t> *targetBlendEnable,
                      be_val<BOOL> *multiWriteEnable,
                      be_val<BOOL> *colorWriteEnable)
{
   auto cb_color_control = reg->cb_color_control.value();
   *rop3 = static_cast<GX2LogicOp::Value>(cb_color_control.ROP3);
   *targetBlendEnable = cb_color_control.TARGET_BLEND_ENABLE;
   *multiWriteEnable = cb_color_control.MULTIWRITE_ENABLE;

   if (cb_color_control.SPECIAL_OP == latte::CB_SPECIAL_DISABLE) {
      *colorWriteEnable = FALSE;
   } else {
      *colorWriteEnable = TRUE;
   }
}

void
GX2SetColorControlReg(GX2ColorControlReg *reg)
{
   auto cb_color_control = reg->cb_color_control.value();
   pm4::write(pm4::SetContextReg { latte::Register::CB_COLOR_CONTROL, cb_color_control.value });
}


void
GX2SetDepthStencilControl(BOOL depthTest,
                          BOOL depthWrite,
                          GX2CompareFunction::Value depthCompare,
                          BOOL stencilTest,
                          BOOL backfaceStencil,
                          GX2CompareFunction::Value frontStencilFunc,
                          GX2StencilFunction::Value frontStencilZPass,
                          GX2StencilFunction::Value frontStencilZFail,
                          GX2StencilFunction::Value frontStencilFail,
                          GX2CompareFunction::Value backStencilFunc,
                          GX2StencilFunction::Value backStencilZPass,
                          GX2StencilFunction::Value backStencilZFail,
                          GX2StencilFunction::Value backStencilFail)
{
   GX2DepthStencilControlReg reg;
   GX2InitDepthStencilControlReg(&reg,
                                 depthTest,
                                 depthWrite,
                                 depthCompare,
                                 stencilTest,
                                 backfaceStencil,
                                 frontStencilFunc,
                                 frontStencilZPass,
                                 frontStencilZFail,
                                 frontStencilFail,
                                 backStencilFunc,
                                 backStencilZPass,
                                 backStencilZFail,
                                 backStencilFail);
   GX2SetDepthStencilControlReg(&reg);
}

void
GX2InitDepthStencilControlReg(GX2DepthStencilControlReg *reg,
                              BOOL depthTest,
                              BOOL depthWrite,
                              GX2CompareFunction::Value depthCompare,
                              BOOL stencilTest,
                              BOOL backfaceStencil,
                              GX2CompareFunction::Value frontStencilFunc,
                              GX2StencilFunction::Value frontStencilZPass,
                              GX2StencilFunction::Value frontStencilZFail,
                              GX2StencilFunction::Value frontStencilFail,
                              GX2CompareFunction::Value backStencilFunc,
                              GX2StencilFunction::Value backStencilZPass,
                              GX2StencilFunction::Value backStencilZFail,
                              GX2StencilFunction::Value backStencilFail)
{
   auto db_depth_control = reg->db_depth_control.value();
   db_depth_control.Z_ENABLE = depthTest;
   db_depth_control.Z_WRITE_ENABLE = depthWrite;
   db_depth_control.ZFUNC = static_cast<latte::DB_FRAG_FUNC>(depthCompare);
   db_depth_control.STENCIL_ENABLE = stencilTest;
   db_depth_control.BACKFACE_ENABLE = backfaceStencil;
   db_depth_control.STENCILFUNC = static_cast<latte::REF_FUNC>(frontStencilFunc);
   db_depth_control.STENCILZPASS = static_cast<latte::DB_STENCIL_FUNC>(frontStencilZPass);
   db_depth_control.STENCILZFAIL = static_cast<latte::DB_STENCIL_FUNC>(frontStencilZFail);
   db_depth_control.STENCILFAIL = static_cast<latte::DB_STENCIL_FUNC>(frontStencilFail);
   db_depth_control.STENCILFUNC_BF = static_cast<latte::REF_FUNC>(backStencilFunc);
   db_depth_control.STENCILZPASS_BF = static_cast<latte::DB_STENCIL_FUNC>(backStencilZPass);
   db_depth_control.STENCILZFAIL_BF = static_cast<latte::DB_STENCIL_FUNC>(backStencilZFail);
   db_depth_control.STENCILFAIL_BF = static_cast<latte::DB_STENCIL_FUNC>(backStencilFail);
   reg->db_depth_control = db_depth_control;
}

void
GX2GetDepthStencilControlReg(GX2DepthStencilControlReg *reg,
                             be_val<BOOL> *depthTest,
                             be_val<BOOL> *depthWrite,
                             be_val<GX2CompareFunction::Value> *depthCompare,
                             be_val<BOOL> *stencilTest,
                             be_val<BOOL> *backfaceStencil,
                             be_val<GX2CompareFunction::Value> *frontStencilFunc,
                             be_val<GX2StencilFunction::Value> *frontStencilZPass,
                             be_val<GX2StencilFunction::Value> *frontStencilZFail,
                             be_val<GX2StencilFunction::Value> *frontStencilFail,
                             be_val<GX2CompareFunction::Value> *backStencilFunc,
                             be_val<GX2StencilFunction::Value> *backStencilZPass,
                             be_val<GX2StencilFunction::Value> *backStencilZFail,
                             be_val<GX2StencilFunction::Value> *backStencilFail)
{
   auto db_depth_control = reg->db_depth_control.value();
   *depthTest = db_depth_control.Z_ENABLE;
   *depthWrite = db_depth_control.Z_WRITE_ENABLE;
   *depthCompare = static_cast<GX2CompareFunction::Value>(db_depth_control.ZFUNC);
   *stencilTest = db_depth_control.STENCIL_ENABLE;
   *backfaceStencil = db_depth_control.BACKFACE_ENABLE;
   *frontStencilFunc = static_cast<GX2CompareFunction::Value>(db_depth_control.STENCILFUNC);
   *frontStencilZPass = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILZPASS);
   *frontStencilZFail = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILZFAIL);
   *frontStencilFail = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILFAIL);
   *backStencilFunc = static_cast<GX2CompareFunction::Value>(db_depth_control.STENCILFUNC_BF);
   *backStencilZPass = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILZPASS_BF);
   *backStencilZFail = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILZFAIL_BF);
   *backStencilFail = static_cast<GX2StencilFunction::Value>(db_depth_control.STENCILFAIL_BF);
}

void
GX2SetDepthStencilControlReg(GX2DepthStencilControlReg *reg)
{
   auto db_depth_control = reg->db_depth_control.value();
   pm4::write(pm4::SetContextReg { latte::Register::DB_DEPTH_CONTROL, db_depth_control.value });
}

void
GX2SetLineWidth(float width)
{
   GX2LineWidthReg reg;
   GX2InitLineWidthReg(&reg, width);
   GX2SetLineWidthReg(&reg);
}

void
GX2InitLineWidthReg(GX2LineWidthReg *reg,
                    float width)
{
   // TODO: GX2InitLineWidthReg
}

void
GX2GetLineWidthReg(GX2LineWidthReg *reg,
                   be_val<float> *width)
{
   // TODO: GX2GetLineWidthReg
}

void
GX2SetLineWidthReg(GX2LineWidthReg *reg)
{
   // TODO: GX2SetLineWidthReg
}

void
GX2SetPointSize(float width,
                float height)
{
   GX2PointSizeReg reg;
   GX2InitPointSizeReg(&reg, width, height);
   GX2SetPointSizeReg(&reg);
}

void
GX2InitPointSizeReg(GX2PointSizeReg *reg,
                    float width,
                    float height)
{
   // TODO: GX2InitPointSizeReg
}

void
GX2GetPointSizeReg(GX2PointSizeReg *reg,
                   be_val<float> *width,
                   be_val<float> *height)
{
   // TODO: GX2GetPointSizeReg
}

void
GX2SetPointSizeReg(GX2PointSizeReg *reg)
{
   // TODO: GX2SetPointSizeReg
}

void
GX2SetPointLimits(float min,
                  float max)
{
   GX2PointLimitsReg reg;
   GX2InitPointLimitsReg(&reg, min, max);
   GX2SetPointLimitsReg(&reg);
}

void
GX2InitPointLimitsReg(GX2PointLimitsReg *reg,
                      float min,
                      float max)
{
   // TODO: GX2InitPointLimitsReg
}

void
GX2GetPointLimitsReg(GX2PointLimitsReg *reg,
                     be_val<float> *min,
                     be_val<float> *max)
{
   // TODO: GX2GetPointLimitsReg
}

void
GX2SetPointLimitsReg(GX2PointLimitsReg *reg)
{
   // TODO: GX2SetPointLimitsReg
}

void
GX2SetPolygonControl(GX2FrontFace::Value frontFace,
                     BOOL cullFront,
                     BOOL cullBack,
                     BOOL polyMode,
                     GX2PolygonMode::Value polyModeFront,
                     GX2PolygonMode::Value polyModeBack,
                     BOOL polyOffsetFrontEnable,
                     BOOL polyOffsetBackEnable,
                     BOOL polyOffsetParaEnable)
{
   GX2PolygonControlReg reg;
   GX2InitPolygonControlReg(&reg,
                            frontFace,
                            cullFront,
                            cullBack,
                            polyMode,
                            polyModeFront,
                            polyModeBack,
                            polyOffsetFrontEnable,
                            polyOffsetBackEnable,
                            polyOffsetParaEnable);
   GX2SetPolygonControlReg(&reg);
}

void
GX2InitPolygonControlReg(GX2PolygonControlReg *reg,
                         GX2FrontFace::Value frontFace,
                         BOOL cullFront,
                         BOOL cullBack,
                         BOOL polyMode,
                         GX2PolygonMode::Value polyModeFront,
                         GX2PolygonMode::Value polyModeBack,
                         BOOL polyOffsetFrontEnable,
                         BOOL polyOffsetBackEnable,
                         BOOL polyOffsetParaEnable)
{
   // TODO: GX2InitPolygonControlReg
}

void
GX2GetPolygonControlReg(GX2PolygonControlReg *reg,
                        be_val<GX2FrontFace::Value> *frontFace,
                        be_val<BOOL> *cullFront,
                        be_val<BOOL> *cullBack,
                        be_val<BOOL> *polyMode,
                        be_val<GX2PolygonMode::Value> *polyModeFront,
                        be_val<GX2PolygonMode::Value> *polyModeBack,
                        be_val<BOOL> *polyOffsetFrontEnable,
                        be_val<BOOL> *polyOffsetBackEnable,
                        be_val<BOOL> *polyOffsetParaEnable)
{
   // TODO: GX2GetPolygonControlReg
}

void
GX2SetPolygonControlReg(GX2PolygonControlReg *reg)
{
   // TODO: GX2SetPolygonControlReg
}

void
GX2SetPolygonOffset(float frontOffset,
                    float frontScale,
                    float backOffset,
                    float backScale,
                    float clamp)
{
   GX2PolygonOffsetReg reg;
   GX2InitPolygonOffsetReg(&reg,
                           frontOffset,
                           frontScale,
                           backOffset,
                           backScale,
                           clamp);
   GX2SetPolygonOffsetReg(&reg);
}

void
GX2InitPolygonOffsetReg(GX2PolygonOffsetReg *reg,
                        float frontOffset,
                        float frontScale,
                        float backOffset,
                        float backScale,
                        float clamp)
{
   // TODO: GX2InitPolygonOffsetReg
}

void
GX2GetPolygonOffsetReg(GX2PolygonOffsetReg *reg,
                       be_val<float> *frontOffset,
                       be_val<float> *frontScale,
                       be_val<float> *backOffset,
                       be_val<float> *backScale,
                       be_val<float> *clamp)
{
   // TODO: GX2GetPolygonOffsetReg
}

void
GX2SetPolygonOffsetReg(GX2PolygonOffsetReg *reg)
{
   // TODO: GX2SetPolygonOffsetReg
}

void
GX2SetScissor(uint32_t x,
              uint32_t y,
              uint32_t width,
              uint32_t height)
{
   GX2ScissorReg reg;
   GX2InitScissorReg(&reg, x, y, width, height);
   GX2SetScissorReg(&reg);
}

void
GX2InitScissorReg(GX2ScissorReg *reg,
                  uint32_t x,
                  uint32_t y,
                  uint32_t width,
                  uint32_t height)
{
   // TODO: GX2InitScissorReg
}

void
GX2GetScissorReg(GX2ScissorReg *reg,
                 be_val<uint32_t> *x,
                 be_val<uint32_t> *y,
                 be_val<uint32_t> *width,
                 be_val<uint32_t> *height)
{
   // TODO: GX2GetScissorReg
}

void
GX2SetScissorReg(GX2ScissorReg *reg)
{
   // TODO: GX2SetScissorReg
}

void
GX2SetTargetChannelMasks(GX2ChannelMask::Value mask0,
                         GX2ChannelMask::Value mask1,
                         GX2ChannelMask::Value mask2,
                         GX2ChannelMask::Value mask3,
                         GX2ChannelMask::Value mask4,
                         GX2ChannelMask::Value mask5,
                         GX2ChannelMask::Value mask6,
                         GX2ChannelMask::Value mask7)
{
   GX2TargetChannelMaskReg reg;
   GX2InitTargetChannelMasksReg(&reg,
                                mask0,
                                mask1,
                                mask2,
                                mask3,
                                mask4,
                                mask5,
                                mask6,
                                mask7);
   GX2SetTargetChannelMasksReg(&reg);
}

void
GX2InitTargetChannelMasksReg(GX2TargetChannelMaskReg *reg,
                             GX2ChannelMask::Value mask0,
                             GX2ChannelMask::Value mask1,
                             GX2ChannelMask::Value mask2,
                             GX2ChannelMask::Value mask3,
                             GX2ChannelMask::Value mask4,
                             GX2ChannelMask::Value mask5,
                             GX2ChannelMask::Value mask6,
                             GX2ChannelMask::Value mask7)
{
   // TODO: GX2InitTargetChannelMasksReg
}

void
GX2GetTargetChannelMasksReg(GX2TargetChannelMaskReg *reg,
                            be_val<GX2ChannelMask::Value> *mask0,
                            be_val<GX2ChannelMask::Value> *mask1,
                            be_val<GX2ChannelMask::Value> *mask2,
                            be_val<GX2ChannelMask::Value> *mask3,
                            be_val<GX2ChannelMask::Value> *mask4,
                            be_val<GX2ChannelMask::Value> *mask5,
                            be_val<GX2ChannelMask::Value> *mask6,
                            be_val<GX2ChannelMask::Value> *mask7)
{
   // TODO: GX2GetTargetChannelMasksReg
}

void
GX2SetTargetChannelMasksReg(GX2TargetChannelMaskReg *reg)
{
   // TODO: GX2SetTargetChannelMasksReg
}

void
GX2SetViewport(float x,
               float y,
               float width,
               float height,
               float nearZ,
               float farZ)
{
   GX2ViewportReg reg;
   GX2InitViewportReg(&reg, x, y, width, height, nearZ, farZ);
   GX2SetViewportReg(&reg);
}

void
GX2InitViewportReg(GX2ViewportReg *reg,
                   float x,
                   float y,
                   float width,
                   float height,
                   float nearZ,
                   float farZ)
{
   // TODO: GX2InitViewportReg
}

void
GX2GetViewportReg(GX2ViewportReg *reg,
                  be_val<float> *x,
                  be_val<float> *y,
                  be_val<float> *width,
                  be_val<float> *height,
                  be_val<float> *nearZ,
                  be_val<float> *farZ)
{
   // TODO: GX2GetViewportReg
}

void
GX2SetViewportReg(GX2ViewportReg *reg)
{
   // TODO: GX2SetViewportReg
}
