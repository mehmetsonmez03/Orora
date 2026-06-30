// ORORA Appearance System Tests

#include <QTest>
#include <QObject>
#include "../orora/metahuman/appearance/hair_style_manager.h"
#include "../orora/metahuman/appearance/clothing_manager.h"

class TestAppearance : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Appearance System Tests başlatılıyor...";
    }

    void test_HairStyleManager_Creation() {
        Orora::Metahuman::HairStyleManager hairManager;
        QVERIFY(true);
    }

    void test_HairStyleManager_Colors() {
        // Tüm renklerin RGBA dönüşümü olmalı
        for (int i = 0; i < static_cast<int>(Orora::Metahuman::HairColor::Custom); i++) {
            auto color = static_cast<Orora::Metahuman::HairColor>(i);
            auto rgba = Orora::Metahuman::HairStyleManager::ColorToRGBA(color);
            
            // Alpha her zaman 1.0 olmalı
            QCOMPARE(rgba.w, 1.0f);
        }
    }

    void test_HairStyleManager_Styles() {
        Orora::Metahuman::HairStyleManager hairManager;
        auto styles = hairManager.GetAvailableStyles();
        
        // En az 10 stil olmalı
        QVERIFY(styles.size() >= 10);
    }

    void test_ClothingManager_Creation() {
        Orora::Metahuman::ClothingManager clothingManager;
        QVERIFY(true);
    }

    void test_ClothingManager_DefaultWardrobe() {
        Orora::Metahuman::ClothingManager clothingManager;
        clothingManager.Initialize();
        clothingManager.LoadDefaultWardrobe();
        
        auto items = clothingManager.GetAllItems();
        
        // Varsayılan kıyafetler yüklenmeli
        QVERIFY(!items.empty());
    }

    void test_ClothingManager_WearItem() {
        Orora::Metahuman::ClothingManager clothingManager;
        clothingManager.Initialize();
        clothingManager.LoadDefaultWardrobe();
        
        bool success = clothingManager.WearItem("white_tshirt");
        QVERIFY(success);
        QVERIFY(clothingManager.IsWearing("white_tshirt"));
    }

    void test_ClothingManager_Outfit() {
        Orora::Metahuman::ClothingManager clothingManager;
        clothingManager.Initialize();
        
        clothingManager.WearOutfit("Günlük");
        
        auto worn = clothingManager.GetWornItems();
        QVERIFY(!worn.empty());
    }

    void test_ClothingManager_CategoryNames() {
        // Türkçe kategori isimleri olmalı
        auto topName = Orora::Metahuman::ClothingManager::GetCategoryName(
            Orora::Metahuman::ClothingCategory::Top);
        
        QCOMPARE(topName, std::string("Üst Giyim"));
    }
};

QTEST_MAIN(TestAppearance)
#include "test_appearance.moc"
