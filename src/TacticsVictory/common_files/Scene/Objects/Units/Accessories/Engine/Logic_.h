// 2021/02/27 10:05:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Unit;


struct CommandEngine { enum E {
    None,
    MoveToNorth,
    MoveToNorthEast,
    MoveToEast,
    MoveToEastSouth,
    MoveToSouth,
    MoveToSouthWest,
    MoveToWest,
    MoveToWestNorth
}; };


//----------------------------------------------------------------------------------------------------------------------
// ��� ���������
//
class Step
{
public:

    struct Type { enum E {
        None,
        Move,                   // �������� �� ����� � �����
        Rotate                  // ������� �� ���� � ����
    }; };

    Step(Type::E t = Type::None) : type(t) {}

    Vector3 endPos;     // ���� ��� �����������, �� ����� �������� �������� �������, ���� ��� �������� - ����������
                        // �����, � ������� ���������� �����������

    Type::E type = Type::None;

    bool IsMovement() const { return type == Type::Move; }
    bool IsRotate() const { return type == Type::Rotate; }
};


//----------------------------------------------------------------------------------------------------------------------
// �������� �������� - ��������� ������������ EngineCalculator
class EngineAlgorithm
{
public:

    bool IsFinished() const { return steps.Empty(); }

    void Clear() { steps.Clear(); }

    Step &GetStep() { return steps.Front(); }

    List<Step> steps;     // ����� �������� ���� ��������� - �� ������� � ����������. ����� �������������
                          // ���������� ���� [0], �� ���������.
};


//----------------------------------------------------------------------------------------------------------------------
//  ����������� �������� ��������, ����������� ��� ���������� �������
class EngineCalculator
{
public:

    void Calculate(Unit &unit, CommandEngine::E command, int count, EngineAlgorithm &algorithm);

private:

    // ���������� false, ���� ������� �� ����� (���� �� ����� ���� �� �������)
    bool CalculateRotate(Unit &unit, CommandEngine::E command, EngineAlgorithm &algorithm);

    void CalculateMovement(Unit &unit, CommandEngine::E command, int count, EngineAlgorithm &algorithm);
};


//----------------------------------------------------------------------------------------------------------------------
// ��������� ��������, ����� ������������ EngineCalculator
class EngineExecutor
{
public:

    struct Result { enum E {
        Running,            // ��� �������� ��������, ��� ������� ���������� ���������� ����
        Finished            // ��� �������� ��������, ��� ���������� ���� ���������
    };
    
        E value;

        Result(E v) : value(v) {}

        bool IsFinished() const { return value == Finished; }
    };

    // ��������� �������� (�.�. �������) ��� ���������. ���� ���������� ���� ��������� - ���������� Result::Finished
    Result Execute(Unit &unit, float dT);

    Result ExecuteRotate(Unit &unit, float dT);

    Result ExecuteMovement(Unit &unit, float dT);
};
