// ORORA Integration Tests

#include <QTest>
#include <QObject>
#include "../orora/orora_app.h"

class TestIntegration : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Integration Tests başlatılıyor...";
    }

    void test_OroraApp_Creation() {
        Orora::OroraApp app;
        QVERIFY(true);
    }

    void test_OroraApp_StateInitial() {
        Orora::OroraApp app;
        QCOMPARE(app.GetState(), Orora::AppState::Uninitialized);
    }

    void test_GenderProfile_Female() {
        Orora::Core::GenderProfileManager profile;
        profile.SetGender(Orora::Core::Gender::Female);
        
        auto currentProfile = profile.GetCurrentProfile();
        QCOMPARE(currentProfile.gender, Orora::Core::Gender::Female);
        
        // Kadın profili daha sıcak olmalı
        QVERIFY(currentProfile.personality.warmth > 0.7f);
    }

    void test_GenderProfile_Male() {
        Orora::Core::GenderProfileManager profile;
        profile.SetGender(Orora::Core::Gender::Male);
        
        auto currentProfile = profile.GetCurrentProfile();
        QCOMPARE(currentProfile.gender, Orora::Core::Gender::Male);
        
        // Erkek profili daha analitik olmalı
        QVERIFY(currentProfile.personality.analytical > 0.7f);
    }

    void test_GenderProfile_TurkishGreeting() {
        Orora::Core::GenderProfileManager profile;
        profile.SetGender(Orora::Core::Gender::Female);
        
        auto greeting = profile.GetAppropriateGreeting("morning");
        
        // Türkçe selamlama içermeli
        QVERIFY(greeting.find("Günaydın") != std::string::npos);
    }

    void test_SystemsAccessible() {
        Orora::OroraApp app;
        
        // Sistemler başlatılmadan null olmalı
        QVERIFY(app.GetSTTEngine() == nullptr);
        QVERIFY(app.GetTTSEngine() == nullptr);
        QVERIFY(app.GetRenderer() == nullptr);
    }
};

QTEST_MAIN(TestIntegration)
#include "test_integration.moc"
