#pragma once
namespace qfcbase
{
    typedef struct input_state
    {
    public:
        input_state() : move_up(false), move_down(false), move_left(false), move_right(false), action(false), run(false), open_menu(false)
        {

        }

        bool move_up, move_down, move_left, move_right;
        bool action, run, open_menu;
    };

    class PlayerInput
    {
    private:
        input_state old_state, current_state;

    protected:
        virtual input_state GetState() = 0;

    public:
        PlayerInput();
        ~PlayerInput();

        void Update(double dt);

        // GamePlay
        bool MoveUp() { return current_state.move_up; }
        bool MoveDown() { return current_state.move_down; }
        bool MoveLeft() { return current_state.move_left; }
        bool MoveRight() { return current_state.move_right; }

        bool Run() { return current_state.run; }
        bool Action() { return current_state.action; }

        // Menu
        bool MenuUp() { return current_state.move_up && !old_state.move_up; }
        bool MenuDown() { return current_state.move_down && !old_state.move_down; }
        bool MenuLeft() { return current_state.move_left && !old_state.move_left; }
        bool MenuRight() { return current_state.move_right && !old_state.move_right; }

        bool MenuOpen() { return current_state.open_menu && !old_state.open_menu; }
        bool MenuAccept() { return current_state.action && !old_state.action; }
        bool MenuCancel() { return current_state.run && !old_state.run; }
    };
}