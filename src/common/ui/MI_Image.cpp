#include "MI_Image.h"

#include "gfx.h"

#include <cmath>

extern SDL_Surface * blitdest;


MI_Image::MI_Image(
        gfxSprite * nspr,
        short x, short y,
        short srcx, short srcy,
        short w, short h,
        short numxframes, short numyframes, short speed, int id )
    : UI_Control(x, y)
    , spr(nspr)
    , iNumXFrames(numxframes)
    , iNumYFrames(numyframes)
    , isrcx(srcx)
    , isrcy(srcy)
    , iw(w)
    , ih(h)
    , iSpeed(speed)
    , iXFrame(srcx)
    , iYFrame(srcy)
{
    this->id = id;
}

void MI_Image::Update()
{
    if (!m_visible)
        return;

    if (iSpeed > 0 && ++iTimer >= iSpeed) {
        iTimer = 0;
        iXFrame += iw;

        if (iXFrame >= iNumXFrames * iw + isrcx) {
            iXFrame = isrcx;
            iYFrame += ih;

            if (iYFrame >= iNumYFrames * ih + isrcy) {
                iYFrame = isrcy;
            }
        }
    }

    if (fPulse) {
        if (++iPulseDelay >= 3) {
            iPulseDelay = 0;

            if (fPulseOut) {
                if (++iPulseValue >= 10) {
                    fPulseOut = false;
                }
            } else {
                if (--iPulseValue <= 0) {
                    fPulseOut = true;
                }
            }
        }
    }

    if (fSwirl) {
        dSwirlRadius -= dSwirlRadiusSpeed;

        if (dSwirlRadius <= 0.0f) {
            fSwirl = false;
        } else {
            dSwirlAngle += dSwirlAngleSpeed;
        }
    }

    if (fBlink) {
        if (++iBlinkCounter > iBlinkInterval) {
            fBlinkShow = !fBlinkShow;
            iBlinkCounter = 0;
        }
    }
}

void MI_Image::Draw()
{
    if (!m_visible || (fBlink && !fBlinkShow))
        return;

    short iXOffset = 0;
    short iYOffset = 0;

    if (fSwirl) {
        iXOffset = (short)(dSwirlRadius * cos(dSwirlAngle));
        iYOffset = (short)(dSwirlRadius * sin(dSwirlAngle));
    }

    const SDL_Rect srcRect {iXFrame, iYFrame, iw, ih};
    if (fPulse) {
        const SDL_Rect dstRect {
            m_pos.x - iPulseValue + iXOffset,
            m_pos.y - iPulseValue + iYOffset,
            iw + (iPulseValue << 1),
            ih + (iPulseValue << 1),
        };
        spr->drawStretch(srcRect, blitdest, dstRect);
    } else {
        if(drawScaledToCenter){

            // e.g. if scale is 70% -> offset x and y 15% 
            // w and h are 70% wich leaves 15% at the other side 
            float inverseScaleHalfWidth = (1 - scaleVal)/2.f * iw;
            float inverseScaleHalfHeight = (1 - scaleVal)/2.f * ih;

            SDL_Rect dstRect {
                m_pos.x + inverseScaleHalfWidth  ,
                m_pos.y + inverseScaleHalfHeight  ,
                iw * scaleVal,
                ih * scaleVal,
            };

            spr->drawStretch(srcRect, blitdest, dstRect);
        }else
            spr->draw(m_pos.x + iXOffset, m_pos.y + iYOffset, srcRect);
    }
}
