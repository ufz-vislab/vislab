#pragma once

#include <stdlib.h>
#include <map>

class CWPEAttrPanel;
class CWPEDiagramView;
class CWPETypeInfo;
class CWPEState;
class CElevationGridBase;
class CWPE3DModel;


class CWPEProjectController
{
private:
	static int m_iCurrentId;
protected:
	static CWPEProjectController *m_pInstance;
	CWPEProjectController(void);

public:
	~CWPEProjectController(void);

	static CWPEProjectController	*Instance();

	void			SetAttrPanel(CWPEAttrPanel*);
	void			SetDiagramView(CWPEDiagramView*);

	int				GetUniqueId();
	CWPEState		GetNewState(const char *wpeTypeName);

	void			InitAddWPE();
	void			FinishAddWPE(CWPEState state, float xpos, float ypos);
	void			RemoveActiveWPE();
	void			ChangeTypeOfActiveWPE(const char *wpeTypeName);
	void			SetActiveWPE(int id);
	void			UnsetActiveWPE(int id);

	void			SetNew3DModelPosition(int id, float xPos, float yPos);

	void			ScaleDown();
	void			ScaleUp();

	void			LoadArealImage(const char *filename);
	void			LoadTerrainModel(const char *filename);
	void			LoadLandscape(const char *filename);
	void			Set3DModelFilename(const char *filename){m_stdstr3DModelFilename = std::string(filename);}

	void			LoadTypes(const char *filename);
	void			AddType(CWPETypeInfo typeInfo);
	CWPETypeInfo	GetTypeInfoByName(const char *typeName);

	float			CalculateEnergyProduction();
	void			SetEnergyProduction();
	void			SetNewWindDirection(float direction);
	void			SetRotorRotation(float rotation);
	void			UseRotorRotation(bool use);

	void			SaveWindparkInfo(const char *filename);
	void			RemoveAllWPEs();

protected:
	CWPEAttrPanel		*m_pAttrPanel;
	CWPEDiagramView		*m_pDiagramView;

	float				m_fCanvasXSize;
	float				m_fCanvasYSize;

	float				m_fDiagramTo3DXTranslate;
	float				m_fDiagramTo3DYTranslate;

	bool				m_bUseRotorRotation;

	std::string			m_stdstr3DModelFilename;

	CElevationGridBase *m_pElevationGrid;

	std::map<std::string, CWPETypeInfo>		m_stdmapWPETypes;
	std::map<int, CWPEState>				m_stdmapWPEStates;
	std::map<int, CWPE3DModel*>				m_stdmap3DModels;

	void Get3DCoordFrom2DCoords(float inX, float inY, float &outX, float &outY, float &outZ);
};
