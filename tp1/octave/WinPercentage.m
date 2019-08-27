args = argv();
filename = args{1};
matches = dlmread(filename, ' ', 1, 1);
teams = dlmread(filename, ' ', [0 0 0 0]);
win = zeros(teams, 1);
played = zeros(teams, 1);
for i = 1 : size(matches)
  teamA = matches(i, 1);
  teamB = matches(i, 3);
  if matches(i, 2) > matches(i,4)
    win(teamA) += 1;
  else %No empate
    win(teamB) += 1;
  endif
  played(teamA) += 1;
  played(teamB) += 1;
endfor
win = win + ones(teams, 1);
played = played + 2*ones(teams,1);
result = win ./ played;

disp(result);
