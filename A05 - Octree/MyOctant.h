/*----------------------------------------------
Programmer: Nik Barbero (nfb1799@rit.edu)
Date: 2021/04
----------------------------------------------*/
#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{

	//System class
	class MyOctant
	{
		static uint m_uOctantCount; //will store the number of octants instantiated
		static uint m_uMaxLevel;//will store the maximum level an octant can go to
		static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

		uint m_uID = 0; //Will store the current ID for this octant
		uint m_uLevel = 0; //Will store the current level of the octant
		uint m_uChildren = 0;// Number of children on the octant (either 0 or 8)

		float m_fSize = 0.0f; //Size of the octant

		MeshManager* m_pMeshMngr = nullptr;//Mesh Manager singleton
		MyEntityManager* m_pEntityMngr = nullptr; //Entity Manager Singleton

		vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
		vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
		vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the octant

		MyOctant* m_pParent = nullptr;// Will store the parent of current octant
		MyOctant* m_pChild[8];//Will store the children of the current octant

		std::vector<uint> m_EntityList; //List of Entities under this octant (Index in Entity Manager)

		MyOctant* m_pRoot = nullptr;//Root octant
		std::vector<MyOctant*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

		

	public: 

		//Constructor with max subdivisions and ideal entity count
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);

		//Constructor with center point in global space and size of the octant
		MyOctant(vector3 a_v3Center, float a_fSize);

		//Copy constructor
		MyOctant(MyOctant const& other);

		//Copy assignment operator
		MyOctant& operator=(MyOctant const& other);

		//Destructor
		~MyOctant(void);

		//Swaps contents of two octants
		void Swap(MyOctant& other);

		//Getters
		float GetSize(void);
		vector3 GetCenterGlobal(void);
		vector3 GetMinGlobal(void);
		vector3 GetMaxGlobal(void);

		//Checks collisions based on bounding object index
		bool IsColliding(uint a_uRBIndex);

		//Displays octant volume specified by index and color
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);

		//Displays octant volume in specified color
		void Display(vector3 a_v3Color = C_YELLOW);

		//Displays leaves in specified color
		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);

		//Clears entity list for each node
		void ClearEntityList(void);

		//Creates 8 octants as children
		void Subdivide(void);

		//Get child specified by index
		MyOctant* GetChild(uint a_nChild);
		
		//Get parent of octant
		MyOctant* GetParent(void);

		//Checks if octant has any children
		bool IsLeaf(void);

		//Checks if octant contains more than a specified number of objects
		bool ContainsMoreThan(uint a_nEntities);

		//Deletes all children and descendants
		void KillBranches(void);

		//Creates tree using subdivisions, max number of objects, and levels
		void ConstructTree(uint a_nMaxLevel = 3);

		//Traverse the tree up to the leafs and sets the objects in them to the index
		void AssignIDtoEntity(void);

		//Get total number of octants
		uint GetOctantCount(void);

	private:

		void Init(void);
		void Release(void);

		//Create list of leaves that contain objects
		void ConstructList(void);

	}; //MyOctant class
}

#endif __MYOCTANTCLASS_H_