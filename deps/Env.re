open Bos;
open Astring;

type envVar = (string, string);
type withDefaults = list(envVar);
type t = String.map(string);

let envWithDefaults: withDefaults = [
  ("BRANCH_NAME", "master"),
  ("DOTFILES_PATH", "$HOME/.dotfiles"),
  ("FNM_PATH", "$HOME/.fnm"),
  ("HOME", ""),
  ("RBENV_PATH", "$HOME/.rbenv/bin"),
];

let replaceWithEnv = ((name, absent): envVar): envVar => (
  name,
  OS.Env.(value(name, string, ~absent)),
);

let make = (): t =>
  envWithDefaults |> List.map(replaceWithEnv) |> String.Map.of_list;

let getVar = (name: string, env) => String.Map.get(name, env);
