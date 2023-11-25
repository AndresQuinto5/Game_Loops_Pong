function calculate_ai_move(ball_position_y, ball_rect_height, paddle_position_y, paddle_height, ai_speed, delta_time)
    local target_y = ball_position_y + (ball_rect_height / 2)
    local paddle_center_y = paddle_position_y + (paddle_height / 2)

    local velocity_y = 0
    if paddle_center_y < target_y then
        velocity_y = ai_speed
    elseif paddle_center_y > target_y then
        velocity_y = -ai_speed
    end

    local new_position_y = paddle_position_y + (velocity_y * delta_time)
    print("LUA - Velocity Y: " .. velocity_y .. ", New Position Y: " .. new_position_y)
    return new_position_y
end