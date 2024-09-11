document.addEventListener('DOMContentLoaded', (event) => {
    document.getElementById("login-form").addEventListener("submit", function (event) {
        event.preventDefault();

        const username = document.getElementById("username").value;
        const password = document.getElementById("password").value;

        const storedUsername = "admin";
        const storedPassword = "admin";

        if (username === storedUsername && password === storedPassword) {
            sessionStorage.setItem("isLoggedIn", true);
            window.location.href = "index.html";
        } else {
            alert("Wrong username or password");
        }
    });
});
