#include "EspDrawer.h"
#include "BattlefieldData.h"


static const DWORD fontColor = 0xff000000;


D3DCOLOR EspDrawer::GetFadedColor(D3DCOLOR from, D3DCOLOR to, float factor) const
{
    if (factor <= 0)
        return from;
    if (factor >= 1)
        return to;

    D3DCOLOR ret;
    unsigned char *rbyte = (unsigned char*)&ret;
    unsigned char *fbyte = (unsigned char*)&from;
    unsigned char *tbyte = (unsigned char*)&to;

    for (int i = 0; i < 4; i++)
        rbyte[i] = (unsigned char)(factor * (tbyte[i] - fbyte[i]) + fbyte[i]);

    return ret;
}


void EspDrawer::SetDevice(IDirect3DDevice9 *pDevice)
{
    DrawerD3D::setContext(pDevice);

    std::vector<hl::VERTEX_3D_COL> boxVerts {
        { -0.5f, -0.5f, 0 },
        { -0.5f, 0.5f, 0 },
        { 0.5f, -0.5f, 0 },
        { 0.5f, 0.5f, 0 },
        { -0.5f, -0.5f, 1 },
        { -0.5f, 0.5f, 1 },
        { 0.5f, -0.5f, 1 },
        { 0.5f, 0.5f, 1 },
    };
    std::vector<unsigned int> boxInds {
        0, 1, 3, 2, 0, 4, 5, 1, 5, 7, 3, 7, 6, 2, 6, 4,
    };
    m_vbBox = allocVertexBuffer(boxVerts);
    m_ibBox = allocIndexBuffer(boxInds);

    std::vector<hl::VERTEX_3D_DIF> arrowVerts {
        { 1, 0, 0, 0x99cc9933 },
        { -0.5f, -0.5f, 0, 0x99eecc33 },
        { -0.5f, 0.5f, 0, 0x99eecc33 },
    };
    std::vector<unsigned int> arrowInds {
        0, 1, 2,
        0, 2, 1,
    };
    m_vbArrow = allocVertexBuffer(arrowVerts);
    m_ibArrow = allocIndexBuffer(arrowInds);

    m_font = allocFont("Arial", 12);
}


void EspDrawer::DrawPlayer(const Player *pPlayer, const CamData *pCam, D3DCOLOR color, bool showDistance, bool showNames) const
{
    hl::Vec3 screenPos;
    project(pPlayer->getPos(), screenPos);

    if (!isInfrontCam(screenPos) || !isOnScreen(screenPos, 100))
        return;

    if (showDistance)
        DrawFont(screenPos.x, screenPos.y, fontColor, "%.1f", D3DXVec3Length(&(pPlayer->getPos()-pCam->getPos())));

    if (showNames)
        DrawFont(screenPos.x, screenPos.y, 0xffff0000, pPlayer->getName());

#ifdef _DEBUG
    DrawFont(screenPos.x, screenPos.y+15, fontColor, "pPlayer: %p", pPlayer->getInternalRep());
    if (pPlayer->getSkeleton())
        DrawFont(screenPos.x, screenPos.y+30, fontColor, "pSkeleton: %p", pPlayer->getSkeleton()->getInternalRep());
    DrawFont(screenPos.x, screenPos.y+45, fontColor, "pos: %.1f %.1f %.1f", pPlayer->getPos().x, pPlayer->getPos().y, pPlayer->getPos().z);
    DrawFont(screenPos.x, screenPos.y+60, fontColor, "vel: %.1f %.1f %.1f", pPlayer->getVel().x, pPlayer->getVel().y, pPlayer->getVel().z);
#endif

    hl::Vec3 screenPosTop;
    project(pPlayer->getPos() + D3DXVECTOR3(0,0,2.2f), screenPosTop);
    D3DXVECTOR2 heightVec(screenPos.x-screenPosTop.x, screenPos.y-screenPosTop.y);
    float screenScale = D3DXVec2Length(&heightVec);

    // bounding box with arrow
    D3DXMATRIX scale, rotate, translate;
    D3DXMatrixScaling(&scale, 1, 1, 2);
    D3DXMatrixRotationZ(&rotate, pPlayer->getRotation());
    D3DXMatrixTranslation(&translate, pPlayer->getPos().x, pPlayer->getPos().y, pPlayer->getPos().z);
    auto worldTransform = scale*rotate*translate;
    drawPrimitive(m_vbBox, m_ibBox, D3DPT_LINESTRIP, worldTransform, color);
    drawPrimitive(m_vbArrow, m_ibArrow, D3DPT_TRIANGLELIST, worldTransform);

    // minimap arrows
	// D3DXMATRIX scaleMm, rotOther, rotMyInverse, transRelativeToMe;
	//D3DXMatrixScaling(&scaleMm, 5, 5, 1);
	//D3DXMatrixRotationZ(&rotOther, -pPlayer->getRotation()+D3DX_PI);
	//D3DXMatrixRotationZ(&rotMyInverse, atan2(pCam->getViewVec().y, pCam->getViewVec().x));
	//D3DXMatrixTranslation(&transRelativeToMe, pCam->getPos().x-pPlayer->getPos().x, pCam->getPos().y-pPlayer->getPos().y, 0);
	//DrawPrimitive(m_vbMmArrow, nullptr, D3DPT_TRIANGLELIST, scaleMm*rotOther*transRelativeToMe*rotMyInverse);
    

    // health bar
    D3DCOLOR healthColor = GetFadedColor(0xccff0000, 0x9900ff00, pPlayer->getHealth());
    D3DCOLOR healthColorDark = GetFadedColor(0xcc770000, 0x99007700, pPlayer->getHealth());
    float hpbarScale = screenScale * 0.4f;
    drawRectFilled(screenPosTop.x - hpbarScale, screenPosTop.y, 2*hpbarScale*pPlayer->getHealth(), 0.4f*hpbarScale, healthColor);
    drawRect(screenPosTop.x - hpbarScale, screenPosTop.y, 2*hpbarScale, 0.4f*hpbarScale, healthColorDark);

    // skeleton
    auto skeleton = pPlayer->getSkeleton();
    if (skeleton)
        for (const auto& i : skeleton->getBones())
            if (!i.ignore)
                drawLineProjected(i.pos, skeleton->getBones()[i.parent].pos, color);
}

void EspDrawer::DrawOwnInfo(const Player *pOwnPlayer, const OwnData *data) const
{
    float centerX = getWidth()/2;
    float centerY = getHeight()/2;
    float barHeight = getHeight()/25;

    if (data)
    {
        // spread
        float spread = data->getSpread();
        drawCircle(centerX, centerY, spread*12.0f, GetFadedColor(0x77aa5500, 0xffff0000, spread/2.0f));

        // reload
        float reload = data->getReload();
        D3DCOLOR reloadColor;
        if (reload < 0.1f)
            reloadColor = GetFadedColor(0x00777777, 0xaa3333cc, reload * 10);
        else if (reload > 0.95f)
            reloadColor = GetFadedColor(0xaa3333cc, 0x00777777, (reload-0.95f) * 20);
        else
            reloadColor = 0xaa3333cc;
        if (reload > 0 && reload < 1) {
            drawRectFilled(centerX-getWidth()/6, centerY+getHeight()/8, reload*getWidth()/3, barHeight, reloadColor);
            drawRect(centerX-getWidth()/6, centerY+getHeight()/8, getWidth()/3, barHeight, reloadColor-0x00111155);
        }

        // overheat
    }

    if (pOwnPlayer)
    {
        // health
        float healthSizeFactor = 3 - 2*sqrt(pOwnPlayer->getHealth());
        float barX = getWidth() - (getWidth()/25) - healthSizeFactor * (getWidth()/7);
        float barWidth = healthSizeFactor * (getWidth()/7);
        D3DCOLOR healthColor = GetFadedColor(0xffff0000, 0x6600ff00, pOwnPlayer->getHealth());
        D3DCOLOR healthColorDark = GetFadedColor(0xff770000, 0x66007700, pOwnPlayer->getHealth());
        drawRectFilled(barX, getHeight()-getHeight()/6, pOwnPlayer->getHealth()*barWidth, barHeight, healthColor);
        drawRect(barX, getHeight()-getHeight()/6, barWidth, barHeight, healthColorDark);
    }
}

void EspDrawer::DrawFont(float x, float y, D3DCOLOR color, std::string format, ...) const
{
    va_list vl;
    va_start(vl, format);
    DrawerD3D::drawFont(m_font, x, y, color, format, vl);
    va_end(vl);
}