#ifndef ESPDRAWER_H
#define ESPDRAWER_H

#include "../hacklib/include/hacklib/DrawerD3D.h"


class Player;
class OwnData;
class CamData;


class EspDrawer : public hl::DrawerD3D
{
public:
    D3DCOLOR GetFadedColor(D3DCOLOR from, D3DCOLOR to, float factor) const;

    void SetDevice(IDirect3DDevice9 *pDevice);

    void DrawPlayer(const Player *pPlayer, const CamData *pCam, D3DCOLOR color, bool showDistance, bool showNames) const;
    void DrawOwnInfo(const Player *pOwnPlayer, const OwnData *data) const;
    void DrawFont(float x, float y, D3DCOLOR color, std::string format, ...) const;

private:
    const hl::VertexBuffer *m_vbBox = nullptr;
    const hl::IndexBuffer *m_ibBox = nullptr;
    const hl::VertexBuffer *m_vbArrow = nullptr;
    const hl::IndexBuffer *m_ibArrow = nullptr;
    const hl::Font *m_font = nullptr;

};

#endif
