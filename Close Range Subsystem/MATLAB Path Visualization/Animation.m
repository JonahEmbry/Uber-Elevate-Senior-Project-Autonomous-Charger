clear; format short e
figure(1); clf

t = linspace(0, 2*pi, 1000)
x = @(t) cos(t) .* (1+.4*cos(30*t));
y = @(t) sin(t) .* (1+.4*cos(30*t));
z = @(t) 0.4*sin(30*t)

plot3(x(t), y(t), z(t), 'k-')
axis equal

hold on
MyDot = plot3(x(t(1)), y(t(1)), z(t(1)), 'bo', 'MarkerFaceColor', 'b')
hold off
M(1) = getframe

for k=2:length(t)
    set(MyDot, 'XData', x(t(k)), 'YData', y(t(k)), 'ZData', z(t(k)))
    drawnow
end