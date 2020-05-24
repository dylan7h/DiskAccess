#ifndef __PATH_MANAGER_H__
#define __PATH_MANAGER_H__

namespace PathManager {
    bool IsExist(TCHAR Path[]);
    bool CreatePath(TCHAR Path[]);
    bool DeletePath(TCHAR Path[]);
};

#endif