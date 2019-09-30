type route =
  | Home
  | About
  | Contact;

type menu =
  | Initial
  | Open
  | Closed;

type formAction =
  | Blur(string)
  | Change(string, string)
  | Validate;

type submissionStatus =
  | Pending
  | Success
  | Failed;

type submissionState = {
  loading: bool,
  message: option(string),
  status: submissionStatus
};

type submissionAction =
  | SubmitMessageRequest(string, string)
  | SubmitMessageSuccess
  | SubmitMessageFailure(string);