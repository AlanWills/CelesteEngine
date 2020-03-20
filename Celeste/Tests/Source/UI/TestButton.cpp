#include "UtilityHeaders/UnitTestHeaders.h"

#include "Mocks/UI/MockButton.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Input/MouseInteractionHandler.h"
#include "Rendering/SpriteRenderer.h"
#include "Audio/AudioSource.h"
#include "Physics/RectangleCollider.h"
#include "DataConverters/UI/ButtonDataConverter.h"
#include "Utils/InputUtils.h"
#include "Registries/ComponentRegistry.h"
#include "AssertCel.h"

using namespace Celeste;
using namespace Celeste::Audio;
using namespace Celeste::Resources;
using namespace Celeste::Physics;
using namespace Celeste::Input;
using namespace Celeste::Rendering;


namespace TestCeleste
{
  using namespace UI;

  CELESTE_TEST_CLASS(TestButton)

#pragma region Registration Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_IsRegisteredWithComponentRegistry)
  {
    Assert::IsTrue(ComponentRegistry::hasComponent<Button>());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_IsAllocatableFromComponentRegistry)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<Component> component = ComponentRegistry::createComponent(Button::type_name(), gameObject);

    Assert::IsNotNull(component);
    Assert::IsNotNull(dynamic_cast<Button*>(component));
    Assert::AreEqual(&gameObject, component->getGameObject());
  }

#pragma endregion

#pragma region Constructor Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Constructor_SetsButtonStateTo_kIdle)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    MockButton button(gameObject);

    Assert::IsTrue(button.getButtonState_Public() == Button::ButtonState::kIdle);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Constructor_LoadsResources)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    MockButton button(gameObject);

    Assert::IsNotNull(button.getDefaultTexture());
    Assert::IsNotNull(button.getHighlightedTexture());
    Assert::IsNotNull(button.getClickedTexture());
    Assert::IsNotNull(button.getClickedSound());
    Assert::IsNotNull(button.getHighlightedSound());
  }

#pragma endregion

#pragma region On Set GameObject Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnSetGameObject_SetsSpriteRendererTexture_ToButtonDefaultTexture)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.findComponent<SpriteRenderer>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsTrue(spriteRenderer->getTexture() == button->getDefaultTexture());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnSetParent_SetsColliderDimensions_AndScalesThemWithWorldScale)
  {
    GameObject gameObject;
    gameObject.getTransform()->setWorldScale(0.5f, 0.25f, 5);
    setUpButtonComponents(gameObject);

    observer_ptr<RectangleCollider> collider = gameObject.findComponent<RectangleCollider>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsTrue(collider->getDimensions() == (button->getDefaultTexture()->getDimensions() * glm::vec2(0.5f, 0.25f)));
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnSetGameObject_SetsAudioSourceToSFX)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<AudioSource> audioSourceSource = gameObject.findComponent<AudioSource>();
    audioSourceSource->setAudioType(AudioType::kMusic);

    Assert::IsTrue(audioSourceSource->getAudioType() == AudioType::kMusic);

    gameObject.addComponent<MockButton>();

    Assert::IsTrue(audioSourceSource->getAudioType() == AudioType::kSFX);
  }

#pragma endregion

#pragma region Update Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Update_ClickedAndMouseOver_SetsButtonStateToHighlighted)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<Collider> collider = gameObject.findComponent<Collider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.findComponent<SpriteRenderer>();

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    button->setButtonState_Public(Button::ButtonState::kIdle);

    Assert::IsTrue(Button::ButtonState::kIdle == button->getButtonState_Public());

    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // Have to make sure we exhaust the click timer
    button->update(100);

    Assert::AreEqual(button->getHighlightedTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::AreEqual(button->getHighlightedTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(Button::ButtonState::kHighlighted == button->getButtonState_Public());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Update_ClickedAndMouseOver_DoesNothing)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<Collider> collider = gameObject.findComponent<Collider>();

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    button->setButtonState_Public(Button::ButtonState::kClicked);
      
    Assert::IsTrue(Button::ButtonState::kClicked == button->getButtonState_Public());

    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // Have to make sure we exhaust the click timer
    button->update(100);

    Assert::IsTrue(Button::ButtonState::kClicked == button->getButtonState_Public());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Update_ClickedAndMouseNotOver_SetsButtonStateToIdle)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<Collider> collider = gameObject.findComponent<Collider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.findComponent<SpriteRenderer>();

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    button->setButtonState_Public(Button::ButtonState::kClicked);

    Assert::IsTrue(Button::ButtonState::kClicked == button->getButtonState_Public());

    getMouse().getTransform().setTranslation(10000, 10000);

    Assert::IsFalse(collider->intersects(glm::vec2(10000, 10000)));

    // Have to make sure we exhaust the click timer
    button->update(100);

    Assert::AreEqual(button->getDefaultTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::AreEqual(button->getDefaultTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(Button::ButtonState::kIdle == button->getButtonState_Public());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_Update_NotClickedAndMouseNotOver_SetsButtonStateToIdle)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<Collider> collider = gameObject.findComponent<Collider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.findComponent<SpriteRenderer>();

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    button->setButtonState_Public(Button::ButtonState::kHighlighted);

    Assert::IsTrue(Button::ButtonState::kHighlighted == button->getButtonState_Public());

    getMouse().getTransform().setTranslation(10000, 10000);

    Assert::IsFalse(collider->intersects(glm::vec2(10000, 10000)));

    // Have to make sure we exhaust the click timer
    button->update(100);

    Assert::AreEqual(button->getDefaultTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::AreEqual(button->getDefaultTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(Button::ButtonState::kIdle == button->getButtonState_Public());
  }

#pragma endregion

#pragma region Subscribe Left Click Callback Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SubscribeLeftClickCallback_WithNullMouseInteractionHandler_DoesNothing)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    MockButton button(gameObject);
    button.subscribeLeftClickCallback([](GameObject&) -> void { });
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SubscribeLeftClickCallback_SingleCallback_SubscribesToInteractionHandler)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<MouseInteractionHandler> mouseInteractionHandler = gameObject.findComponent<MouseInteractionHandler>();

    observer_ptr<MouseInteractionHandler> handler = gameObject.findComponent<MouseInteractionHandler>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    gameObject.findComponent<Collider>()->setHitByRay(true);
    button->setButtonState_Public(Button::ButtonState::kClicked);

    Assert::AreEqual(handler, mouseInteractionHandler);
    Assert::IsTrue(gameObject.findComponent<Collider>()->isHitByRay());

    bool called = false;
    button->subscribeLeftClickCallback([&called](GameObject&) -> void { called = true; });
    handler->getOnLeftButtonUpEvent().invoke(gameObject);

    Assert::IsTrue(called);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SubscribeLeftClickCallback_MultipleCallbacks_SuubscribesToInteractionHandler)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<MouseInteractionHandler> mouseInteractionHandler = gameObject.findComponent<MouseInteractionHandler>();

    observer_ptr<MouseInteractionHandler> handler = gameObject.findComponent<MouseInteractionHandler>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    gameObject.findComponent<Collider>()->setHitByRay(true);
    button->setButtonState_Public(Button::ButtonState::kClicked);

    Assert::AreEqual(handler, mouseInteractionHandler);
    Assert::IsTrue(gameObject.findComponent<Collider>()->isHitByRay());

    bool called = false;
    bool called2 = false;
    button->subscribeLeftClickCallback(
      [&called](GameObject&) -> void { called = true; },
      [&called2](GameObject&) -> void { called2 = true; });
    handler->getOnLeftButtonUpEvent().invoke(gameObject);

    Assert::IsTrue(called);
    Assert::IsTrue(called2);
  }

#pragma endregion

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseEnter_SetsTextureToHighlighted_UpdatesSpriteRendererDimensions_PlaysHighlightedAudio_AndSetsStateToHighlighted)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);
    observer_ptr<Collider> collider = gameObject.findComponent<Collider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.findComponent<SpriteRenderer>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.findComponent<AudioSource>();

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // Have to set up mouse interaction script
    gameObject.update(0);
    gameObject.handleInput();
    button->update(0);

    Assert::AreEqual(button->getHighlightedTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::AreEqual(button->getHighlightedTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kHighlighted);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeave_SetsTextureToDefault)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();  // Make sure this is last so it doesn't create components

    getMouse().getTransform().setTranslation(0, 0);

    Assert::IsTrue(collider->intersects(glm::vec2(0, 0)));

    // Have to set up mouse interaction script
    gameObject.update(0);
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    gameObject.handleInput();
    button->update(0);

    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kHighlighted);
    Assert::AreEqual(button->getHighlightedTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(audioSourceSource->isPlaying());

    collider->setHitByRay(false);

    Assert::IsFalse(collider->isHitByRay());

    gameObject.handleInput();
    button->update(0);

    Assert::AreEqual(button->getDefaultTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::AreEqual(button->getDefaultTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kIdle);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeftClick_WhenNotClicked_SetsTextureToClicked_UpdatesSpriteRendererDimensions_PlaysClickedAudio_AndSetsStateToClicked)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    button->setButtonState_Public(Button::ButtonState::kIdle);

    Assert::AreEqual(button->getDefaultTexture(), spriteRenderer->getTexture());
    Assert::IsFalse(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kIdle);

    // Have to set up mouse interaction script
    gameObject.update(0);
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // This must go before the button handle input
    // Simulate a mouse click
    simulateMouseButtonClicked(MouseButton::kLeft);
      
    Assert::IsTrue(getMouse().isButtonClicked(MouseButton::kLeft));

    gameObject.handleInput();

    Assert::AreEqual(button->getClickedTexture()->getDimensions(), spriteRenderer->getDimensions());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kClicked);
    Assert::AreEqual(button->getClickedTexture(), spriteRenderer->getTexture());
    Assert::IsTrue(audioSourceSource->isPlaying());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeftButtonReleased_ButtonNotClicked_HandlesEvent)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    bool called = false;
    button->subscribeLeftClickCallback([&called](GameObject&) -> void
    {
      called = true;
    });

    button->setButtonState_Public(Button::ButtonState::kIdle);

    Assert::IsTrue(button->getDefaultTexture() == spriteRenderer->getTexture());
    Assert::IsFalse(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kIdle);

    // Have to set up mouse interaction script
    gameObject.update(0);
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // This must go before the button handle input
    // Simulate a mouse click
    simulateMouseButtonClicked(MouseButton::kLeft);

    Assert::IsTrue(getMouse().isButtonClicked(MouseButton::kLeft));

    button->setButtonState_Public(Button::ButtonState::kIdle);

    gameObject.handleInput();

    Assert::IsFalse(called);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeftButtonReleased_ColliderNotHitByRay_HandlesEvent)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    bool called = false;
    button->subscribeLeftClickCallback([&called](GameObject&) -> void
    {
      called = true;
    });

    button->setButtonState_Public(Button::ButtonState::kIdle);

    Assert::IsTrue(button->getDefaultTexture() == spriteRenderer->getTexture());
    Assert::IsFalse(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kIdle);

    // Have to set up mouse interaction script
    gameObject.update(0);
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // This must go before the button handle input
    // Simulate a mouse click
    simulateMouseButtonClicked(MouseButton::kLeft);

    Assert::IsTrue(getMouse().isButtonClicked(MouseButton::kLeft));

    collider->setHitByRay(false);

    Assert::IsFalse(collider->isHitByRay());

    gameObject.handleInput();

    Assert::IsFalse(called);
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeftButtonReleased_ButtonNotClicked_DoesNotChangeButtonState)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    bool called = false;
    button->subscribeLeftClickCallback([&called](GameObject&) -> void
    {
      called = true;
    });

    button->setButtonState_Public(Button::ButtonState::kIdle);

    Assert::IsTrue(button->getDefaultTexture() == spriteRenderer->getTexture());
    Assert::IsFalse(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kIdle);

    // Have to set up mouse interaction script
    gameObject.update(0);

    Assert::IsFalse(collider->isHitByRay());

    // This must go before the button handle input
    // Simulate a mouse click
    simulateMouseButtonClicked(MouseButton::kLeft);

    Assert::IsTrue(getMouse().isButtonClicked(MouseButton::kLeft));

    gameObject.handleInput();

    Assert::IsTrue(Button::ButtonState::kIdle == button->getButtonState_Public());

    button->setButtonState_Public(Button::ButtonState::kHighlighted);

    Assert::IsTrue(Button::ButtonState::kHighlighted == button->getButtonState_Public());

    simulateMouseButtonReleased(MouseButton::kLeft);
    gameObject.handleInput();

    Assert::IsFalse(called);
    Assert::IsTrue(Button::ButtonState::kHighlighted == button->getButtonState_Public());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_OnMouseLeftButtonReleased_ButtonClicked_SetsButtonStateTo_kIdle)
  {
    GameObject gameObject;
    observer_ptr<RectangleCollider> collider = gameObject.addComponent<RectangleCollider>();
    observer_ptr<SpriteRenderer> spriteRenderer = gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    observer_ptr<AudioSource> audioSourceSource = gameObject.addComponent<AudioSource>();
    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    bool called = false;
    button->subscribeLeftClickCallback([&called](GameObject&) -> void
    {
      called = true;
    });

    button->setButtonState_Public(Button::ButtonState::kHighlighted);

    Assert::IsTrue(button->getDefaultTexture() == spriteRenderer->getTexture());
    Assert::IsFalse(audioSourceSource->isPlaying());
    Assert::IsTrue(button->getButtonState_Public() == Button::ButtonState::kHighlighted);

    // Have to set up mouse interaction script
    gameObject.update(0);
    collider->setHitByRay(true);

    Assert::IsTrue(collider->isHitByRay());

    // This must go before the button handle input
    // Simulate a mouse click
    simulateMouseButtonClicked(MouseButton::kLeft);

    Assert::IsTrue(getMouse().isButtonClicked(MouseButton::kLeft));

    gameObject.handleInput();

    Assert::IsTrue(Button::ButtonState::kClicked == button->getButtonState_Public());

    simulateMouseButtonReleased(MouseButton::kLeft);
    gameObject.handleInput();

    Assert::IsTrue(called);
    Assert::IsTrue(Button::ButtonState::kIdle == button->getButtonState_Public());
  }

#pragma region Set Default Texture Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetDefaultTexture_InputtingInvalidPath_SetsDefaultTextureToNull)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getDefaultTexture());

    button->setDefaultTexture("WubbaLubbaDubDub.png");

    Assert::IsNull(button->getDefaultTexture());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetDefaultTexture_InputtingValidPath_SetsDefaultTextureToCorrectTexture)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getDefaultTexture());

    // Set to different texture
    button->setDefaultTexture(ButtonDataConverter::getHighlightedTextureDefaultPath());

    Assert::IsNotNull(button->getDefaultTexture());
    Assert::AreEqual(getResourceManager().load<Texture2D>(ButtonDataConverter::getHighlightedTextureDefaultPath()), button->getDefaultTexture());
  }

#pragma endregion

#pragma region Set Highlighted Texture Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetHighlightedTexture_InputtingInvalidPath_SetsHighlightedTextureToNull)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getHighlightedTexture());

    button->setHighlightedTexture("WubbaLubbaDubDub.png");

    Assert::IsNull(button->getHighlightedTexture());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetHighlightedTexture_InputtingValidPath_SetsHighlightedTextureToCorrectTexture)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getHighlightedTexture());

    // Set to different texture
    button->setHighlightedTexture(ButtonDataConverter::getClickedTextureDefaultPath());

    Assert::IsNotNull(button->getHighlightedTexture());
    Assert::AreEqual(getResourceManager().load<Texture2D>(ButtonDataConverter::getClickedTextureDefaultPath()), button->getHighlightedTexture());
  }

#pragma endregion

#pragma region Set Clicked Texture Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetClickedTexture_InputtingInvalidPath_SetsClickedTextureToNull)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getClickedTexture());

    button->setClickedTexture("WubbaLubbaDubDub.png");

    Assert::IsNull(button->getClickedTexture());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetClickedTexture_InputtingValidPath_SetsClickedTextureToCorrectTexture)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getClickedTexture());

    // Set to different texture
    button->setClickedTexture(ButtonDataConverter::getDefaultTextureDefaultPath());

    Assert::IsNotNull(button->getClickedTexture());
    Assert::AreEqual(getResourceManager().load<Texture2D>(ButtonDataConverter::getDefaultTextureDefaultPath()), button->getClickedTexture());
  }

#pragma endregion

#pragma region Set Highlighted Sound Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetHighlightedSound_InputtingInvalidPath_SetsHighlightedSoundToNull)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getHighlightedSound());

    button->setHighlightedSound("WubbaLubbaDubDub.wav");

    Assert::IsNull(button->getHighlightedSound());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetHighlightedSound_InputtingValidPath_SetsHighlightedSoundToCorrectSound)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getHighlightedSound());

    // Set to different sound
    button->setHighlightedSound(ButtonDataConverter::getClickedSoundDefaultPath());

    Assert::IsNotNull(button->getHighlightedSound());
    Assert::AreEqual(getResourceManager().load<Sound>(ButtonDataConverter::getClickedSoundDefaultPath()), button->getHighlightedSound());
  }

#pragma endregion

#pragma region Set Clicked Sound Tests

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetClickedSound_InputtingInvalidPath_SetsClickedSoundToNull)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getClickedSound());

    button->setClickedSound("WubbaLubbaDubDub.wav");

    Assert::IsNull(button->getClickedSound());
  }

  //------------------------------------------------------------------------------------------------
  TEST_METHOD(Button_SetClickedSound_InputtingValidPath_SetsClickedSoundToCorrectSound)
  {
    GameObject gameObject;
    setUpButtonComponents(gameObject);

    observer_ptr<MockButton> button = gameObject.addComponent<MockButton>();

    Assert::IsNotNull(button->getClickedSound());

    // Set to different sound
    button->setClickedSound(ButtonDataConverter::getHighlightedSoundDefaultPath());

    Assert::IsNotNull(button->getClickedSound());
    Assert::AreEqual(getResourceManager().load<Sound>(ButtonDataConverter::getHighlightedSoundDefaultPath()), button->getClickedSound());
  }

#pragma endregion

#pragma region Utility Functions

  //------------------------------------------------------------------------------------------------
  void TestButton::setUpButtonComponents(GameObject& gameObject)
  {
    gameObject.addComponent<RectangleCollider>();
    gameObject.addComponent<SpriteRenderer>();
    gameObject.addComponent<MouseInteractionHandler>();
    gameObject.addComponent<AudioSource>();
  }

  //------------------------------------------------------------------------------------------------
  void TestButton::setUpButtonComponents(observer_ptr<GameObject> gameObject)
  {
    setUpButtonComponents(*gameObject);
  }

#pragma endregion

  };
}