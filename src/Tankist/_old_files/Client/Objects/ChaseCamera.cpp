#include "stdafx.h"
#include "ChaseCamera.h"



ChaseCamera::ChaseCamera() : FrustumCamera(2.0F, 1.0F)
{
    targetModel = nullptr;
}


ChaseCamera::~ChaseCamera()
{
}


void ChaseCamera::Move(void)
{
    Model *model = GetTargetModel();
    if (model)
    {
        CollisionData	data;

        VehicleController *controller = static_cast<VehicleController *>(model->GetController());
        float azm = controller->GetModelAzimuth();
        float alt = controller->GetModelAltitude();

        /// ����� �� ������������ ��������� ������� ��������� ��� ������ ������ � ����������� �� �����������, ������� ������� �����.

        float cp = Cos(alt);
        float sp = Sin(alt);

        float ct = Cos(azm);
        float st = Sin(azm);

        Vector3D view(ct * cp, st * cp, sp);
        Vector3D right(st, -ct, 0.0F);
        Vector3D down = view % right;

        /// �� ���������� ���������� ������ ������ ������, �� �� �� ����, ����� ������ ������ ����� ����� ���������, ������� �� ������� ������� ��������� �� ������������.

        const Point3D& position = model->GetWorldPosition();
        Point3D p1(position.x, position.y, position.z + 0.5F);
        Point3D p2 = p1 - view * 20.0F;

        if (GetWorld()->DetectCollision(p1, p2, 0.3F, Pi::CollisionKind::Camera, &data))
        {
            /// ��� - �� ������, ��� ��� ����������� ������ �����  ������.

            float t = data.param;
            p2 = p1 * (1.0F - t) + p2 * t;
        }

        /// ���������� ��������� ������ � ����������.

        SetNodeTransform(right, down, view, p2);
    }
}
