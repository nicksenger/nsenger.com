type route =
  | Home
  | About
  | Contact;

type menu =
  | Initial
  | Open
  | Closed;

type formActions =
  | Blur(string)
  | Change(string, string)
  | Validate;