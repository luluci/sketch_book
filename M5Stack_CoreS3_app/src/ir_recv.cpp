#include "ir_recv.hpp"
#include "pinassign.hpp"

#include <IRutils.h>

// ir_recv_info_t ir_recv_pool[ir_recv_pool_size];
String ir_recv_pool[ir_recv_pool_size];
size_t ir_recv_pool_pos;
size_t ir_recv_pool_head;
size_t ir_recv_pool_count;

IRrecv ir_recv(PIN_IR_RECV, ir_recv_buffer_size, ir_recv_timeout, true);
decode_results ir_recv_results;

void ir_recv_init()
{
    // Perform a low level sanity checks that the compiler performs bit field
    // packing as we expect and Endianness is as we expect.
    assert(irutils::lowLevelSanityCheck() == 0);

    ir_recv_pool_pos = 0;
    ir_recv_pool_head = 0;
    ir_recv_pool_count = 0;

    ir_recv.setTolerance(kTolerance); // Override the default tolerance.
    ir_recv.enableIRIn();             // Start the receiver
}

bool ir_recv_check()
{
    bool result = false;

    if (ir_recv.decode(&ir_recv_results))
    {

        // ir_recv_pool[ir_recv_pool_pos] = resultToHexidecimal(&ir_recv_results);
        ir_recv_pool[ir_recv_pool_pos] = resultToHumanReadableBasic(&ir_recv_results);
        ir_recv_pool_pos++;
        if (ir_recv_pool_pos >= ir_recv_pool_size)
        {
            ir_recv_pool_pos = 0;
        }
        else
        {
            ir_recv_pool_count++;
        }
        if (ir_recv_pool_pos == ir_recv_pool_head)
        {
            ir_recv_pool_head++;
            if (ir_recv_pool_head >= ir_recv_pool_size)
            {
                ir_recv_pool_head = 0;
            }
        }

        // RTOSのWDTクリア？
        // yield();

        result = true;
    }

    return result;
}
