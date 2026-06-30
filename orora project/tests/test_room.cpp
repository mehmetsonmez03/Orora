// ORORA Room System Tests

#include <QTest>
#include <QObject>
#include "../orora/metahuman/room/orora_room.h"
#include "../orora/metahuman/room/navigation_system.h"

class TestRoom : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Room System Tests başlatılıyor...";
    }

    void test_OroraRoom_Creation() {
        Orora::Metahuman::OroraRoom room;
        QVERIFY(true);
    }

    void test_OroraRoom_SetupDefault() {
        Orora::Metahuman::OroraRoom room;
        Orora::Metahuman::RoomConfig config;
        
        QVERIFY(room.Initialize(config));
        room.SetupDefaultRoom();
        
        // Varsayılan mobilyalar oluşturulmalı
        auto furniture = room.GetAllFurniture();
        QVERIFY(!furniture.empty());
    }

    void test_OroraRoom_FurnitureTypes() {
        Orora::Metahuman::OroraRoom room;
        Orora::Metahuman::RoomConfig config;
        room.Initialize(config);
        room.SetupDefaultRoom();
        
        // Her mobilya tipi olmalı
        QVERIFY(!room.GetFurnitureByType(Orora::Metahuman::FurnitureType::Bed).empty());
        QVERIFY(!room.GetFurnitureByType(Orora::Metahuman::FurnitureType::Wardrobe).empty());
        QVERIFY(!room.GetFurnitureByType(Orora::Metahuman::FurnitureType::Couch).empty());
    }

    void test_NavigationSystem_Creation() {
        Orora::Metahuman::NavigationSystem nav;
        QVERIFY(true);
    }

    void test_NavigationSystem_PathFinding() {
        Orora::Metahuman::NavigationSystem nav;
        nav.Initialize();
        nav.SetRoomBounds(10.0f, 10.0f);
        nav.BuildNavMesh();
        
        Orora::Metahuman::Vec3 start = {1.0f, 0.0f, 1.0f};
        Orora::Metahuman::Vec3 end = {5.0f, 0.0f, 5.0f};
        
        auto path = nav.FindPath(start, end);
        
        // Yol bulunmalı
        QVERIFY(!path.empty());
    }

    void test_NavigationSystem_Obstacle() {
        Orora::Metahuman::NavigationSystem nav;
        nav.Initialize();
        nav.SetRoomBounds(10.0f, 10.0f);
        
        // Engel ekle
        Orora::Metahuman::Vec3 obsMin = {4.0f, 0.0f, 4.0f};
        Orora::Metahuman::Vec3 obsMax = {6.0f, 2.0f, 6.0f};
        nav.AddObstacle(obsMin, obsMax);
        
        // Engel noktası yürünemez olmalı
        Orora::Metahuman::Vec3 testPos = {5.0f, 0.0f, 5.0f};
        QVERIFY(!nav.IsPositionWalkable(testPos));
    }
};

QTEST_MAIN(TestRoom)
#include "test_room.moc"
