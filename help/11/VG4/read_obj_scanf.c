����

sscanf(Buf, "%i%i%i", &a, &b, &c); 

� ������ ������� ������� � ��������:
1/2/3
1//3
1/2
1
  sscanf(Buf, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
  sscanf(Buf, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
  sscanf(Buf, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
  sscanf(Buf, "%i %i %i", &a, &b, &c); 
  a--;
  b--;
  c--;

