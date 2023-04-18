//========================================
// ���C�g�̏���
// Author:��a�c��
//========================================
#include "light.h"
//========================================
// �O���[�o���ϐ�
//========================================
D3DLIGHT9 g_lihgt;//���C�g�̏��
//���C�g�̏��
static Light g_lihgtInformation[NUM_LIGHT] =
{
 { D3DXVECTOR3(-0.9f , -0.9f , 0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// ��E�O
 { D3DXVECTOR3(0.2f , 0.9f , -0.2f)	,D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// ��
 { D3DXVECTOR3(0.9f , -0.9f , -0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// �E�E��
};

//========================================
// ����������
//========================================
void InitLight(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;//�ݒ�p�����x�N�g��

	//���C�g�̏����N���A����
	ZeroMemory(&g_lihgt, sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	g_lihgt.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̊g�U����ݒ�
		g_lihgt.Diffuse = g_lihgtInformation[nCntLight].col;

		//���C�g�̕�����ݒ�
		vecDir = g_lihgtInformation[nCntLight].rot;

		//�x�N�g���𐳋K��
		D3DXVec3Normalize(&vecDir, &vecDir);

		g_lihgt.Direction = vecDir;
		
		g_lihgt.Range = 300.0f;
		g_lihgt.Position = D3DXVECTOR3(30.0f, 70.0f, 500.0f);

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_lihgt);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
//========================================
// �I������
//========================================
void UninitLight(void)
{

}
//========================================
// �X�V����
//========================================
void UpdateLight(void)
{

}