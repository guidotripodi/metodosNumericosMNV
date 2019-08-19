#!/usr/bin/env -S octave -qf
args = argv();
filename = args{1};
matches = dlmread(filename, ' ', 1, 1);
teams = dlmread(filename, ' ', [0 0 0 0]);
win = zeros(teams, 1);
lost = zeros(teams, 1);
N = zeros(teams);
for i = 1 : size(matches)
  teamA = matches(i, 1);
  teamB = matches(i, 3);
  N(teamA, teamB) += 1;
  N(teamB, teamA) += 1;
  if matches(i, 2) > matches(i,4)
    win(teamA) += 1;
    lost(teamB) += 1;
  else %No empate
    win(teamB) += 1;
    lost(teamA) += 1;
  endif
endfor
played = win + lost;

C = -N + diag(2 + played);
B = 1 + (win - lost)/2;
result = C\B;
disp(result);


