#pragma once

class CNavigationDiagramView;
class CNavigationAttrPanel;
class CElevationGridBase;

class CNavigationProjectController
{
protected:
	static CNavigationProjectController *m_pInstance;
	CNavigationProjectController(void);
public:
	static CNavigationProjectController *Instance();
	~CNavigationProjectController(void);

	void SetDiagramView(CNavigationDiagramView *view);
	void SetAttrPanel(CNavigationAttrPanel *attrPanel);

	void SetNewFromPosition(double dXPos, double dYPos);
	void SetNewAtPosition(double dXPos, double dYPos);
	void SetShowFromAt3DView();
	void SetShowFromAtDistance();
	void SetHeightAboutGround(double height);

	void LoadArealImage(const char *filename);
	void LoadTerrainModel(const char *filename);
	void ScaleUp();
	void ScaleDown();

protected:
	void Get3DCoordFrom2DCoords(double inX, double inY, double &outX, double &outY, double &outZ);

	CNavigationDiagramView *m_pNavigationDiagramView;
	CNavigationAttrPanel *m_pNavigationAttrPanel;

	CElevationGridBase *m_pElevationGrid;

	double				m_dDiagramTo3DXTranslate;
	double				m_dDiagramTo3DYTranslate;
	double				m_dCanvasXSize;
	double				m_dCanvasYSize;

	double				m_dFromX;
	double				m_dFromY;
	double				m_dFromZ;
	double				m_dAtX;
	double				m_dAtY;
	double				m_dAtZ;
	double				m_dHeightAboutGround;
};
