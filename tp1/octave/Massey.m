format('long');
args = argv();
filename = args{1};
matches = dlmread(filename, ' ', 1, 1);
teams = dlmread(filename, ' ', [0 0 0 0]);
D = zeros(teams,1);
M = zeros(teams);
for i = 1 : size(matches)
  teamA = matches(i, 1);
  teamB = matches(i, 3);
  M(teamA, teamA) += 1;
  M(teamB, teamB) += 1;
  M(teamA, teamB) -= 1;
  M(teamB, teamA) -= 1;
  D(teamA) += matches(i,2);
  D(teamA) -= matches(i,4);
  D(teamB) += matches(i,4);
  D(teamB) -= matches(i,2);
endfor
disp(D);
result = M\D;
disp(result);
