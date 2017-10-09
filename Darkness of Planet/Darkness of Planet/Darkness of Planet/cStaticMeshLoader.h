#pragma once
class cObjectNode;
class cHankGrid;
class cStaticMeshLoader
{
private:
	vector<string>	m_meshFileName;
	vector<string>	m_meshPath;
	vector<int>		m_meshId;
	

public:
	cStaticMeshLoader();
	~cStaticMeshLoader();

	void ScriptLoader(const string& fullPath, std::list<cObjectNode*>& Nodes);

	void ObjPlacement(ifstream & file, std::list<cObjectNode*>& cNodes);

	void ObjPlacementSRTin(ifstream & file, cModel& pModel, std::list<cObjectNode*>& cNodes);

	void CreateMesh(ifstream& file);


};

