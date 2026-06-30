/**
 * @file test_field_theory.cpp
 * @brief Unit tests for Field Theory modules
 * 
 * Tests:
 * - Hyperbolic Space (Poincaré embeddings)
 * - Semantic Field (Gaussian Processes)
 * - Holographic Memory
 * - Field Dynamics (Reaction-Diffusion)
 * - Unified Field Engine
 */

#include <QTest>
#include <QDebug>
#include <cmath>
#include <vector>

// Field Theory headers
#include "../orora/orora_core/core/field_theory/hyperbolic_space.h"
#include "../orora/orora_core/core/field_theory/semantic_field.h"
#include "../orora/orora_core/core/field_theory/holographic_memory.h"
#include "../orora/orora_core/core/field_theory/field_dynamics.h"
#include "../orora/orora_core/core/field_theory/unified_field_engine.h"

using namespace orora::field_theory;

class TestFieldTheory : public QObject {
    Q_OBJECT

private slots:
    // === Hyperbolic Space Tests ===
    
    void testHyperbolicPointCreation() {
        HyperbolicPoint p(64);
        QCOMPARE(p.dimension(), size_t(64));
        QVERIFY(p.isValid());  // Zero vector is valid
    }
    
    void testHyperbolicDistance() {
        HyperbolicSpace space(4);
        
        HyperbolicPoint origin(4);
        HyperbolicPoint p1(std::vector<double>{0.1, 0.0, 0.0, 0.0});
        HyperbolicPoint p2(std::vector<double>{0.2, 0.0, 0.0, 0.0});
        
        double dist1 = space.distance(origin, p1);
        double dist2 = space.distance(origin, p2);
        
        QVERIFY(dist2 > dist1);  // Farther point = larger distance
        QVERIFY(dist1 > 0.0);
    }
    
    void testMobiusAddition() {
        HyperbolicSpace space(4);
        
        HyperbolicPoint a(std::vector<double>{0.1, 0.1, 0.0, 0.0});
        HyperbolicPoint b(std::vector<double>{0.1, -0.1, 0.0, 0.0});
        
        auto result = space.mobiusAdd(a, b);
        
        QVERIFY(result.isValid());  // Result should be inside ball
        QVERIFY(result.norm() < 1.0);
    }
    
    void testHyperbolicConceptHierarchy() {
        HyperbolicSpace space(16);
        
        space.addConcept("root", "Root concept");
        space.addConcept("child1", "First child", "root");
        space.addConcept("child2", "Second child", "root");
        space.addConcept("grandchild", "Grandchild", "child1");
        
        auto concepts = space.getConcepts();
        QCOMPARE(concepts.size(), size_t(4));
        
        QVERIFY(space.isAncestor("root", "grandchild"));
        QVERIFY(space.isAncestor("child1", "grandchild"));
        QVERIFY(!space.isAncestor("child2", "grandchild"));
    }
    
    // === Semantic Field Tests ===
    
    void testGaussianProcessPrior() {
        SemanticField field(16);
        
        std::vector<double> query(16, 0.0);
        auto pred = field.predict(query);
        
        QCOMPARE(pred.mean, 0.0);  // Prior mean is zero
        QVERIFY(pred.variance > 0.0);
    }
    
    void testGPPosteriorUpdate() {
        SemanticField field(8);
        
        SemanticPoint obs;
        obs.embedding = std::vector<double>(8, 0.5);
        field.addObservation(obs, 1.0);
        
        auto pred = field.predict(obs.embedding);
        QVERIFY(std::abs(pred.mean - 1.0) < 0.1);  // Close to observed value
        QVERIFY(pred.variance < 1.0);  // Reduced uncertainty at observed point
    }
    
    void testKernelComputation() {
        SemanticField field(4, KernelType::RBF);
        
        std::vector<double> x1{0.0, 0.0, 0.0, 0.0};
        std::vector<double> x2{0.0, 0.0, 0.0, 0.0};
        std::vector<double> x3{1.0, 1.0, 1.0, 1.0};
        
        // Same point: max kernel value
        double k_same = field.kernel(x1, x2);
        double k_diff = field.kernel(x1, x3);
        
        QVERIFY(k_same > k_diff);
    }
    
    // === Holographic Memory Tests ===
    
    void testHolographicEncoding() {
        HolographicMemory memory(256);
        
        auto vec = memory.randomVector();
        QCOMPARE(vec.dimension, size_t(256));
        QVERIFY(std::abs(vec.norm() - 1.0) < 0.01);  // Normalized
    }
    
    void testConvolutionCorrelation() {
        HolographicMemory memory(128);
        
        auto a = memory.randomVector();
        auto b = memory.randomVector();
        
        // Bind: c = a ⊛ b
        auto c = memory.convolve(a, b);
        
        // Unbind: a' = c ⊛ b*
        auto b_inv = memory.inverse(b);
        auto a_retrieved = memory.correlate(c, b);
        
        // Retrieved should be similar to original
        double sim = memory.similarity(a, a_retrieved);
        QVERIFY(sim > 0.3);  // Some similarity (noisy retrieval)
    }
    
    void testMemoryRetrievalByContent() {
        HolographicMemory memory(256);
        
        auto vec1 = memory.vectorFromText("apple");
        auto vec2 = memory.vectorFromText("banana");
        
        memory.encode("apple", vec1, 1.0);
        memory.encode("banana", vec2, 1.0);
        
        auto results = memory.retrieveByContent(vec1, 2, 0.3);
        QVERIFY(!results.empty());
        QCOMPARE(results[0].memory_id, QString("apple").toStdString());
    }
    
    // === Field Dynamics Tests ===
    
    void testFieldInitialization() {
        FieldDynamics field(32, 32, ReactionDiffusionModel::GrayScott);
        
        QCOMPARE(field.getWidth(), size_t(32));
        QCOMPARE(field.getHeight(), size_t(32));
    }
    
    void testFieldEvolution() {
        FieldDynamics field(16, 16, ReactionDiffusionModel::GrayScott);
        
        field.initializeUniform(1.0, 0.0);
        field.addPerturbation(8, 8, 3.0, 0.5, 0.25);
        
        double energy_before = field.totalEnergy();
        field.evolve(100);
        double energy_after = field.totalEnergy();
        
        // Field should change during evolution
        QVERIFY(std::abs(energy_after - energy_before) > 0.01);
    }
    
    void testPatternDetection() {
        FieldDynamics field(32, 32, ReactionDiffusionModel::GrayScott);
        
        field.initializeRandom(0.5, 0.1);
        field.evolve(500);  // Let patterns develop
        
        auto pattern = field.analyzePattern();
        QVERIFY(!pattern.type.empty());
    }
    
    // === Unified Field Engine Tests ===
    
    void testUnifiedFieldInitialization() {
        UnifiedFieldEngine engine(32);
        
        auto state = engine.getCurrentState();
        QVERIFY(state.cognitive_load >= 0.0);
        QVERIFY(state.cognitive_load <= 1.0);
    }
    
    void testThoughtGeneration() {
        UnifiedFieldEngine engine(16);
        
        engine.focusAttention("test_concept");
        auto thought = engine.generateThought();
        
        QVERIFY(!thought.semantic_embedding.empty());
    }
    
    void testCrossSystemIntegration() {
        auto engine = UnifiedFieldEngineFactory::createForLanguage();
        
        engine->addConcept("root");
        engine->addConcept("child", "root");
        
        engine->storeMemory("test_key", "test_content");
        auto result = engine->retrieveMemory("test_key");
        
        QVERIFY(result.confidence > 0.0);
    }
    
    void testFactoryPresets() {
        auto lang_engine = UnifiedFieldEngineFactory::createForLanguage();
        auto creative_engine = UnifiedFieldEngineFactory::createForCreativity();
        auto analysis_engine = UnifiedFieldEngineFactory::createForAnalysis();
        
        QVERIFY(lang_engine != nullptr);
        QVERIFY(creative_engine != nullptr);
        QVERIFY(analysis_engine != nullptr);
    }
};

QTEST_MAIN(TestFieldTheory)
#include "test_field_theory.moc"
