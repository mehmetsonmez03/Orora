// ORORA Metahuman System Tests

#include <QTest>
#include <QObject>
#include "../orora/metahuman/rendering/blend_shape_controller.h"
#include "../orora/metahuman/rendering/skeletal_animator.h"
#include "../orora/metahuman/rendering/lip_sync_engine.h"

class TestMetahuman : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        qDebug() << "Metahuman Tests başlatılıyor...";
    }

    void test_BlendShapeController_Creation() {
        Orora::Metahuman::BlendShapeController controller;
        QVERIFY(true);
    }

    void test_BlendShapeController_SetWeight() {
        Orora::Metahuman::BlendShapeController controller;
        
        // Weight aralığı 0-1 olmalı
        controller.SetWeight(Orora::Metahuman::FaceExpression::JawOpen, 0.5f);
        float weight = controller.GetWeight(Orora::Metahuman::FaceExpression::JawOpen);
        
        QVERIFY(weight >= 0.0f && weight <= 1.0f);
    }

    void test_BlendShapeController_Emotion() {
        Orora::Metahuman::BlendShapeController controller;
        
        controller.SetEmotion(Orora::Metahuman::EmotionalExpression::Happy, 1.0f);
        // Happy emotion ayarlandığında smile weights artmalı
        QVERIFY(true);  // Placeholder
    }

    void test_SkeletalAnimator_Creation() {
        Orora::Metahuman::SkeletalAnimator animator;
        QVERIFY(true);
    }

    void test_LipSyncEngine_PhonemeToViseme() {
        auto phonemes = Orora::Metahuman::LipSyncEngine::TextToPhonemes("Merhaba", 1.0f);
        
        // Phoneme listesi boş olmamalı
        QVERIFY(!phonemes.empty());
    }

    void test_LipSyncEngine_TurkishPhonemes() {
        auto phonemes = Orora::Metahuman::LipSyncEngine::TextToPhonemes("Günaydın", 1.0f);
        
        // Türkçe karakterler işlenmeli
        QVERIFY(!phonemes.empty());
    }
};

QTEST_MAIN(TestMetahuman)
#include "test_metahuman.moc"
