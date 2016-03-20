#include "../game.h"
#include "../engine/encoding.h"
#include "../engine/util.h"

std::ostream &operator<<(std::ostream &os, game_state_t* const &game_state) {
  return os << "initialized " << game_state->initialized << std::endl <<
  "seed " << game_state->seed << std::endl <<
  "num_calls_to_rand " << game_state->num_calls_to_rand << std::endl <<
  "can_fire " << game_state->can_fire << std::endl <<
  "laser_index " << game_state->laser_index << std::endl <<
  "time_since_last_spawn " << game_state->time_since_last_spawn << std::endl;
}

std::ostream &operator<<(std::ostream &os, controller_t const &controller) {
  return os << (controller.right_pressed ? ">" : ".") <<
  " " << (controller.left_pressed ? "<" : ".") <<
  " " << (controller.up_pressed ? "^" : ".") <<
  " " << (controller.down_pressed ? "\\/" : ".") <<
  " " << (controller.jump_pressed ? "o" : ".");
}

const rotations PLAYER_ROTATIONS_PER_SECOND = 1;

int main() {
  frame_t frame;
  read_hex_encoded((uint*) &frame, sizeof(frame));

  printf("dt: %f\n", frame.dt);
  std::cout << &(frame.game_state) << std::endl;
  std::cout << frame.controller << std::endl;

  printf("player direction: %f\n", frame.game_state.player.direction);

  double delta_direction;
  if (frame.controller.right_pressed) {
    delta_direction -= PLAYER_ROTATIONS_PER_SECOND * frame.dt;
  }
  if (frame.controller.left_pressed) {
    delta_direction += PLAYER_ROTATIONS_PER_SECOND * frame.dt;
  }

  printf("wrap(%f + %f, 0, 1)\n", frame.game_state.player.direction, delta_direction);
  printf("= %f\n", wrap(frame.game_state.player.direction + delta_direction, 0, 1));

  update(frame.dt, nullptr, frame.controller)
}