extern void VER_Print();

#define VER_Major 0
#define VER_Minor 0
#define VER_Patch 1
#define VER_String() ((char[]){VER_Major + '0', '.', VER_Minor + '0', '.', VER_Patch + '0', '\0'})
