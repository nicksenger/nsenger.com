[@react.component]
let make = (~visible: bool) =>
  <section className=(visible ? "sio__name" : "sio__name sio__name--hidden")>
    <img src="static/prof.jpeg" className="sio__image" />
    <h3 className="sio__title"> (React.string("Nick Senger")) </h3>
    <div className="sio__social-container">
      <a href="mailto:dev@nsenger.com" className="sio__social-link">
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="25"
          height="25"
          viewBox="0 0 30 30"
        >
          <path
            fill="currentColor"
            d="M15 14.451l8-4.48V9.6c0-.88-.721-1.6-1.6-1.6H8.6C7.72 8 7 8.72 7 9.6v.371l8 4.48z"
          />
          <path
            fill="currentColor"
            d="M15 16.836l-8-4.96V20.4c0 .879.72 1.6 1.6 1.6h12.8c.879 0 1.6-.721 1.6-1.6v-8.525l-8 4.961z"
          />
        </svg>
      </a>
      <a href="https://github.com/nicksenger" className="sio__social-link">
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="25"
          height="25"
          viewBox="0 0 30 30"
        >
          <path
            fill="currentColor"
            d="M15 7.202a7.8 7.8 0 00-2.465 15.2c.39.072.532-.168.532-.375 0-.186-.007-.676-.011-1.327-2.169.472-2.627-1.046-2.627-1.046-.354-.9-.866-1.141-.866-1.141-.708-.484.053-.475.053-.475.783.056 1.195.805 1.195.805.696 1.191 1.826.848 2.27.648.071-.505.272-.849.495-1.043-1.732-.197-3.553-.866-3.553-3.855 0-.852.304-1.548.803-2.093-.08-.197-.348-.99.077-2.064 0 0 .655-.21 2.145.8a7.468 7.468 0 011.953-.263c.662.003 1.33.089 1.953.263 1.489-1.01 2.143-.8 2.143-.8.426 1.074.158 1.867.078 2.064.5.545.802 1.241.802 2.093 0 2.996-1.823 3.656-3.562 3.85.28.24.529.717.529 1.443 0 1.043-.01 1.885-.01 2.141 0 .208.141.451.537.375A7.802 7.802 0 0015 7.202z"
          />
        </svg>
      </a>
      <a href="https://www.linkedin.com/in/nicksenger" className="sio__social-link">
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="25"
          height="25"
          viewBox="0 0 30 30"
        >
          <path
            fill="currentColor"
            d="M21.814 7H8.181C7.529 7 7 7.517 7 8.153v13.692C7 22.482 7.529 23 8.181 23h13.633c.653 0 1.186-.518 1.186-1.154V8.153C23 7.517 22.467 7 21.814 7zM12 21h-2v-7h2v7zm-1-7.65a1.35 1.35 0 110-2.7 1.35 1.35 0 010 2.7zM20 21h-2v-3.195c0-.906-.498-1.77-1.52-1.77S15 16.721 15 17.738V21h-2v-7h2v1.261s.292-.861 2.057-.861C18.82 14.4 20 15.916 20 18.019V21z"
          />
        </svg>
      </a>
    </div>
  </section>;