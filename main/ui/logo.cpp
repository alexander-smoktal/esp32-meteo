#include "logo.h"

#include "ui.h"

LV_IMAGE_DECLARE(logo_image);

Logo::Logo()
{
}

Logo::~Logo()
{
}

void Logo::attach(lv_obj_t *parent)
{
    m_logo_image = lv_image_create(parent);
    /* Assuming a File system is attached to letter 'A'
     * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
    // lv_image_set_src(m_logo_image, "F:/flash/logo.png");
    lv_image_set_src(m_logo_image, &logo_image);
    lv_obj_align(m_logo_image, LV_ALIGN_CENTER, 0, 0);
}

void Logo::detach()
{
    lv_obj_clean(m_logo_image->parent);
    m_logo_image = nullptr;
}

void Logo::fade_in(uint32_t time)
{
    lv_obj_fade_in(m_logo_image, time, 0);
}

void Logo::fade_out(uint32_t time)
{
    lv_obj_fade_out(m_logo_image, time, 0);
}