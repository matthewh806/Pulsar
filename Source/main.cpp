/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class PulsarApplication  : public JUCEApplication
{
public:
    //==============================================================================
    PulsarApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        class MainWindow    : public DocumentWindow
        {
        public:
            MainWindow (MainComponent& content)
            : DocumentWindow (ProjectInfo::projectName, Desktop::getInstance().getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId), DocumentWindow::allButtons)
            {
                setUsingNativeTitleBar (true);
                setContentOwned (new MainComponent(), true);
                
                addKeyListener(getCommandManager().getKeyMappings());

               #if JUCE_IOS || JUCE_ANDROID
                setFullScreen (true);
               #else
                setResizable (true, true);
                centreWithSize (getWidth(), getHeight());
               #endif

                setVisible (true);
            }

            void closeButtonPressed() override
            {
                JUCEApplication::getInstance()->systemRequestedQuit();
            }
            
        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
        };
        
        mCommandManager.registerAllCommandsForTarget(this);
        mMainWindow = std::make_unique<MainWindow>(mContent);
        
        #if JUCE_MAC
                juce::PopupMenu extraAppleMenuItems;
                extraAppleMenuItems.addCommandItem(&mCommandManager, CommandIds::AudioMidiSettings);
                
                juce::MenuBarModel::setMacMainMenu(&mMenuModel, &extraAppleMenuItems);
        #endif
    }

    void shutdown() override
    {
        mMainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }
    
    enum CommandIds
    {
        AudioMidiSettings = 0x0010
    };
    
    juce::ApplicationCommandTarget* getNextCommandTarget() override
    {
        return nullptr;
    }
    
    void getAllCommands(juce::Array<juce::CommandID>& commands) override
    {
        const juce::CommandID ids[] =
        {
            CommandIds::AudioMidiSettings,
        };
        
        commands.addArray(ids, numElementsInArray(ids));
    }
    
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override
    {
        switch(commandID)
        {
            case CommandIds::AudioMidiSettings:
            {
                result.setInfo(juce::translate("Audio/MIDI settings"), juce::translate("Open the Audio/MIDI settings panel"), "Application", 0);
                result.defaultKeypresses.add(juce::KeyPress(',', juce::ModifierKeys::commandModifier, 0));
                break;
            }
        }
    }
    
    bool perform(juce::ApplicationCommandTarget::InvocationInfo const& info) override
    {
        switch(info.commandID)
        {
            case CommandIds::AudioMidiSettings:
            {
                mContent.showAudioSettings();
                return true;
            }
        }
        
        return false;
    }
    
    class MainMenuModel
    : public juce::MenuBarModel
    {
    public:
        MainMenuModel() = default;
        ~MainMenuModel() override = default;
        
        juce::StringArray getMenuBarNames() override
        {
            return {};
        }
        
        juce::PopupMenu getMenuForIndex(int index, juce::String const& name) override
        {
            juce::ignoreUnused(name);
            
            juce::PopupMenu menu;
            if(name == "File")
            {
                menu.addCommandItem(&getCommandManager(), CommandIds::AudioMidiSettings);
            }
            
            return menu;
        }
        
        void menuItemSelected(int itemId, int index) override
        {
            juce::ignoreUnused(itemId, index);
        }
    };
    
    static PulsarApplication& getApp()
    {
        return *dynamic_cast<PulsarApplication*>(JUCEApplication::getInstance());
    }

    static juce::ApplicationCommandManager& getCommandManager()
    {
        return getApp().mCommandManager;
    }

private:
    std::unique_ptr<juce::DocumentWindow> mMainWindow;
    MainComponent mContent;
    
    MainMenuModel mMenuModel;
    juce::ApplicationCommandManager mCommandManager;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (PulsarApplication)
