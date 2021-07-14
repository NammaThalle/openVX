#include <TI/tivx.h>

void openvx_app(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    tivxInit();
    openvx_app(argc, argv);
    tivxDeInit();

    return 0;
}
