document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll("form[data-confirm]").forEach((form) => {
    form.addEventListener("submit", (event) => {
      const label = form.dataset.confirm || "submit";
      const ok = window.confirm(`Ready to ${label}?`);
      if (!ok) {
        event.preventDefault();
      }
    });
  });

  const page = document.body.dataset.page || "";
  document.querySelectorAll(".nav a").forEach((link) => {
    if (page && link.getAttribute("href").includes(page)) {
      link.classList.add("active");
    }
  });
});
