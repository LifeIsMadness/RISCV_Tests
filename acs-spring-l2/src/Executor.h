
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        switch(instr->_type)
        {
            case IType::Alu: {
                instr->_data = getAluResult(instr);
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Ld:
            {
                instr->_addr = getAluResult(instr);
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::St:
            {
                instr->_addr = getAluResult(instr);
                instr->_data = instr->_src2Val;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Csrr:
            {
                instr->_data = instr->_csrVal;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Csrw:
            {
                instr->_data = instr->_src1Val;
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::Auipc:
            {
                instr->_data = ip + instr->_imm.value();
                instr->_nextIp = ip + 4;
                break;
            }
            case IType::J:
            {
                instr->_data = ip + 4;
            }
            case IType::Br:
            {
                if (getBranchResult(instr))
                    instr->_nextIp = ip + instr->_imm.value();
                else
                    instr->_nextIp = ip + 4;
                break;
            }
            case IType::Jr:
            {
                instr->_data = ip + 4;
                if (getBranchResult(instr))
                    instr->_nextIp = instr->_imm.value() + instr->_src1Val;
                else
                    instr->_nextIp = ip + 4;
                break;
            }
        }
    }

private:

    Word getAluResult (InstructionPtr& instr)
    {
        Word firstVal, secondVal;
        bool is_valid = isValid(instr, firstVal, secondVal);

        if (is_valid) {
            switch (instr->_aluFunc)
            {
                case AluFunc::Add:
                    return firstVal + secondVal;
                case AluFunc::Sub:
                    return firstVal - secondVal;
                case AluFunc::And:
                    return firstVal & secondVal;
                case AluFunc::Or:
                    return firstVal | secondVal;
                case AluFunc::Xor:
                    return firstVal ^ secondVal;
                case AluFunc::Slt:
                {
                    int first_value = firstVal, second_value = secondVal;
                    return first_value < second_value;
                }
                case AluFunc::Sltu:
                    return firstVal < secondVal;
                case AluFunc::Sll:
                    return firstVal << (secondVal % 32);
                case AluFunc::Srl:
                    return firstVal >> (secondVal % 32);
                case AluFunc::Sra:
                {
                    int number = firstVal;
                    number = number >> (secondVal % 32);
                    return Word(number);
                }
            }
        }
    }

    bool isValid(const InstructionPtr &instr, Word &firstVal, Word &secondVal)  {
        if (instr->_src1.has_value())
            firstVal = instr->_src1Val;
        else return false;

        if (instr->_imm.has_value())
            secondVal = instr->_imm.value();
        else if (instr->_src2.has_value())
            secondVal = instr->_src2Val;
        else return false;

        return true;
    }

    bool getBranchResult(InstructionPtr& instr)
    {
        Word firstVal, secondVal;
        if (instr->_src1.has_value())
            firstVal = instr->_src1Val;
        if (instr->_src2.has_value())
            secondVal = instr->_src2Val;

        switch (instr->_brFunc)
        {
            case BrFunc :: Eq:
            {
                return firstVal == secondVal;
            }
            case BrFunc :: Neq:
            {
                return firstVal != secondVal;
            }
            case BrFunc :: Lt:
            {
                int first_value = firstVal, second_value = secondVal;
                return first_value < second_value;
            }
            case BrFunc :: Ltu:
            {
                return firstVal < secondVal;
            }
            case BrFunc :: Ge:
            {
                int firstValue = firstVal, secondValue = secondVal;
                return firstValue >= secondValue;
            }
            case BrFunc :: Geu:
            {
                return firstVal >= secondVal;
            }
            case BrFunc :: AT:
            {
                return true;
            }
            case BrFunc :: NT:
            {
                return false;
            }
        }
    }

};

#endif // RISCV_SIM_EXECUTOR_H