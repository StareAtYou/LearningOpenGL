#include "../include/common.h"


void showMat4(const glm::mat4 &mat)
{
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}