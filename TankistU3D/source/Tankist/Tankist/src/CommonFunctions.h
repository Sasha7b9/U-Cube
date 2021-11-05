#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CreateListFiles();

/// ��������� ������ � ������ nameModel �� �������� ���� node
void AddModelToNode(Node *node, char *nameModel, const Vector3 &offset);
/// ���������� ������� � ����� box
void GetDimensionsCenter(const BoundingBox &box, Vector3 &dimensions, Vector3 &center, float scale);


