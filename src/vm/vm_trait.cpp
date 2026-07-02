#include "vm/vm.h"
namespace vm {

bool VM::handleTraitOp(CallFrame& frame, uint8_t instruction) {
    switch (instruction) {
        case OP_TRAIT: {
            uint8_t nameIdx = frame.function->chunk.code[frame.ip++];
            std::string name = std::get<std::string>(frame.function->chunk.constants[nameIdx]);
            ClassObject* klass = allocate<ClassObject>(name);
            klass->isTrait = true;
            push(klass);
            return true;
        }

        default:
            return false;
    }
}

}  // namespace vm
